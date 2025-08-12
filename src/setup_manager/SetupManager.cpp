#include <ArduinoJson.h>
#include "SetupManager.h"
#include "index_html.h"
#include "../../Features.h"
#include "../../Container.h"

#include "esp_bt.h"

#if FEATURE_SCREEN_128x64
//  #include "../screen/128x64/SetupInfoScreen.h"
#endif

#include <NimBLEDevice.h>
#include <string>

#include <Arduino.h>
#include <ESPmDNS.h>

#define DEVICE_MODEL "DefaultSoapDispenser"

// BLE provisioning service and characteristics UUIDs
static const char* PROV_SERVICE_UUID    = "12345678-1234-1234-1234-1234567890a0";
static const char* PROV_CHAR_MODEL_UUID = "12345678-1234-1234-1234-1234567890a1";
static const char* PROV_CHAR_SSID_UUID  = "12345678-1234-1234-1234-1234567890a2";
static const char* PROV_CHAR_PASS_UUID  = "12345678-1234-1234-1234-1234567890a3";
static const char* PROV_CHAR_STATUS_UUID= "12345678-1234-1234-1234-1234567890a4";

// LED blink state for BLE advertising

static bool bleActive = false;
static unsigned long lastBlink = 0;
static bool ledState = false;

// BLE server handle & advertising watchdog
static NimBLEServer* gServer = nullptr;
static unsigned long lastAdvCheck = 0;

// Global characteristic pointers for reliable routing in callbacks
static NimBLECharacteristic* gChModel  = nullptr;
static NimBLECharacteristic* gChSsid   = nullptr;
static NimBLECharacteristic* gChPass   = nullptr;
static NimBLECharacteristic* gChStatus = nullptr;

static std::string gDeviceName; // shared BLE/MDNS hostname (without .local)

class ServerCallbacks : public NimBLEServerCallbacks {
  void onConnect(NimBLEServer* pServer) {
    Serial.println("BLE: client connected");
    // stop blinking while connected (optional)
    bleActive = false;
    digitalWrite(LED_BUILTIN, HIGH);
  }
  void onDisconnect(NimBLEServer* pServer) {
    Serial.println("BLE: client disconnected, restarting advertising");
    // resume advertising so device appears again without power cycle
    if (pServer && pServer->getAdvertising()) {
      pServer->getAdvertising()->start();
    } else {
      NimBLEDevice::getAdvertising()->start();
    }
    // resume blinking
    bleActive = true;
    lastBlink = millis();
    ledState = false;
    digitalWrite(LED_BUILTIN, ledState);
  }
};

class ProvCallbacks : public NimBLECharacteristicCallbacks {
  void onWrite(NimBLECharacteristic* pChar) {
    Serial.printf("BLE: onWrite char uuid=%s handle=%u len=%u ptr=%p\n",
                  pChar->getUUID().toString().c_str(),
                  (unsigned int)pChar->getHandle(),
                  (unsigned int)pChar->getValue().length(),
                  (void*)pChar);

    StorageManager& storage = Container::getInstance()->getStorage();

    // Route by pointer to avoid UUID/handle mismatches
    if (pChar == gChModel) {
      String val = String((char*)pChar->getValue().data());
      storage.setItem("device_model", val);
      Serial.println("BLE: model set to " + val);
      return;
    }

    if (pChar == gChSsid) {
      String val = String((char*)pChar->getValue().data());
      storage.setItem("wifi_ssid", val);
      Serial.print("BLE: SSID received: '");
      Serial.print(val);
      Serial.println("'");
      return;
    }

    if (pChar == gChPass) {
      String val = String((char*)pChar->getValue().data());
      storage.setItem("wifi_password", val);
      Serial.printf("BLE: Password received (len=%u)\n", (unsigned int)val.length());

      // Notify the client that we are starting connection
      if (gChStatus) { gChStatus->setValue("CONNECTING"); gChStatus->notify(); }

      String targetSsid = storage.getItem("wifi_ssid", "");
      Serial.print("BLE: attempting WiFi connect to '");
      Serial.print(targetSsid);
      Serial.println("' in STA mode");

      // Attempt WiFi connection after receiving password
      WiFi.softAPdisconnect(true);
      if (!gDeviceName.empty()) {
        WiFi.setHostname(gDeviceName.c_str());
      }
      WiFi.mode(WIFI_STA);
      WiFi.begin(targetSsid.c_str(), storage.getItem("wifi_password", "").c_str());

      unsigned long start = millis();
      bool ok = false;
      while (millis() - start < 120000) {
        if (WiFi.status() == WL_CONNECTED) { ok = true; break; }
        delay(500);
      }

      unsigned long elapsed = millis() - start;
      if (ok) {
        Serial.print("BLE: WiFi connected. IP: ");
        Serial.println(WiFi.localIP());
        Serial.printf("BLE: RSSI: %d dBm\n", WiFi.RSSI());
        Serial.printf("BLE: connect elapsed: %lu ms\n", elapsed);
      } else {
        Serial.printf("BLE: WiFi connect FAILED (status=%d), elapsed: %lu ms\n", (int)WiFi.status(), elapsed);
      }

      // Notify final status to the client
      if (gChStatus) {
        Serial.print("BLE: notifying status to client: ");
        Serial.println(ok ? "OK" : "ERROR");
        gChStatus->setValue(ok ? "OK" : "ERROR");
        gChStatus->notify();
      }

      // If connected, start mDNS so the browser can discover us by hostname
      if (ok) {
        const char* host = !gDeviceName.empty() ? gDeviceName.c_str() : "dispenser";
        if (!MDNS.begin(host)) {
          Serial.println("mDNS: start FAILED");
        } else {
          MDNS.addService("http", "tcp", 80);
          Serial.printf("mDNS: started, browse http://%s.local/\n", host);
        }

        // Do NOT mark setup complete here; wait until /setup POST saves settings
      }
      return;
    }

    Serial.println("BLE: onWrite from unknown characteristic (no route)");
  }
};

SetupManager::SetupManager(): _server(80), _clientConnected(false) {
}

String SetupManager::name() const {
    return "Setup";
}

IService::Type SetupManager::type() const {
    return IService::Type::Setup;
}

ServiceValue SetupManager::getValue(String key) const {
    return ServiceValue();
}

bool SetupManager::execute(String fnName, JsonVariant args) {
    return false;
}

void SetupManager::begin() {
    StorageManager& _storage = Container::getInstance()->getStorage();
    IService *_screenManager = Container::getInstance()->getService("Screen");

    if (_storage.getItem("initial_setup", "true") != "true") {
        Serial.println("SetupManager: Skipped (not in setup mode)");
        return;
    }

    Serial.println("SetupManager: Setup started");

    // Initialize BLE provisioning
    Serial.println("SetupManager: BLE provisioning start");

    // Free Classic BT memory (we only use BLE)
    esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT);

    String mac = WiFi.macAddress(); // e.g. "AA:BB:CC:DD:EE:FF"
    mac.replace(":", "");          // "AABBCCDDEEFF"
    String tail = mac.substring(mac.length() - 4); // "EEFF"
    std::string devName = std::string("dispenser-") + std::string(tail.c_str());
    gDeviceName = devName; // keep for later (e.g., mDNS after WiFi connect)

    bool inited = NimBLEDevice::init(devName);
    Serial.printf("NimBLE init: %s\n", inited ? "OK" : "FAIL");
    if(!inited){ Serial.println("BLE init failed"); }
    Serial.print("BLE: device name: ");
    Serial.println(devName.c_str());

    // Increase BLE TX power
    NimBLEDevice::setPower(ESP_PWR_LVL_P9);
    Serial.println("BLE: TX power set to P9");
    const char* bleName = devName.c_str();
    NimBLEServer* bleServer = NimBLEDevice::createServer();
    bleServer->setCallbacks(new ServerCallbacks());
    gServer = bleServer;
    NimBLEService* provService = bleServer->createService(PROV_SERVICE_UUID);

    gChModel = provService->createCharacteristic(
        PROV_CHAR_MODEL_UUID,
        NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE
    );
    gChModel->setCallbacks(new ProvCallbacks());
    gChModel->setValue(DEVICE_MODEL); // initialize model value from macro
    Serial.printf("BLE: MODEL  handle: %u (uuid=%s, value='%s', ptr=%p)\n",
                  (unsigned int)gChModel->getHandle(), PROV_CHAR_MODEL_UUID, DEVICE_MODEL, (void*)gChModel);

    gChSsid = provService->createCharacteristic(
      PROV_CHAR_SSID_UUID,
      NIMBLE_PROPERTY::WRITE
    );
    gChSsid->setCallbacks(new ProvCallbacks());
    Serial.printf("BLE: SSID   handle: %u (uuid=%s, ptr=%p)\n",
                  (unsigned int)gChSsid->getHandle(), PROV_CHAR_SSID_UUID, (void*)gChSsid);

    gChPass = provService->createCharacteristic(
      PROV_CHAR_PASS_UUID,
      NIMBLE_PROPERTY::WRITE
    );
    gChPass->setCallbacks(new ProvCallbacks());
    Serial.printf("BLE: PASS   handle: %u (uuid=%s, ptr=%p)\n",
                  (unsigned int)gChPass->getHandle(), PROV_CHAR_PASS_UUID, (void*)gChPass);

    gChStatus = provService->createCharacteristic(
      PROV_CHAR_STATUS_UUID,
      NIMBLE_PROPERTY::NOTIFY
    );
    // Client Characteristic Configuration descriptor for notifications
    gChStatus->createDescriptor("2902");
    Serial.printf("BLE: STATUS handle: %u (uuid=%s, ptr=%p), CCCD expected: %u\n",
                  (unsigned int)gChStatus->getHandle(), PROV_CHAR_STATUS_UUID, (void*)gChStatus,
                  (unsigned int)(gChStatus->getHandle() + 1));

    provService->start();
    Serial.println("BLE: provisioning service started");

    NimBLEAdvertising* adv = NimBLEDevice::getAdvertising();
    // 20–40 ms advertising interval for better discoverability
    adv->setMinInterval(0x20); // 20ms
    adv->setMaxInterval(0x40); // 40ms
    Serial.printf("BLE: ADV interval min=%u max=%u (units of 0.625ms)\n", 0x20, 0x40);

    NimBLEAdvertisementData advData;
    advData.setFlags(0x06);  // General Discoverable, BR/EDR not supported
    advData.setName(bleName);
    advData.setCompleteServices(NimBLEUUID(PROV_SERVICE_UUID));
    adv->setAdvertisementData(advData);

    NimBLEAdvertisementData scanResp;
    scanResp.setName(bleName);
    scanResp.setCompleteServices(NimBLEUUID(PROV_SERVICE_UUID));
    adv->setScanResponseData(scanResp);

    adv->start();
    Serial.print("BLE advertising with name: ");
    Serial.println(bleName);
    Serial.println("BLE: advertising started");

    // Start LED blinking to indicate BLE is active
    pinMode(LED_BUILTIN, OUTPUT);
    bleActive = true;
    lastBlink = millis();
    ledState = false;
    digitalWrite(LED_BUILTIN, ledState);

    WiFi.softAP("SoapDispenser");
    Serial.println("SetupManager: SoftAP started");
    Serial.print("SetupManager: AP IP: ");
    Serial.println(WiFi.softAPIP());
    Serial.print("SetupManager: SSID: ");
    Serial.println(WiFi.softAPSSID());

    // Serve the embedded index HTML
    _server.on("/", HTTP_GET, [this]() {
        _server.send(200, "text/html", INDEX_HTML);
        _clientConnected = true;
        _lastInteraction = millis();
    });

    _server.on("/schema", HTTP_GET, [this]() {
        // Build unified top-level JSON Schema (draft-07)
        DynamicJsonDocument doc(24576);
        JsonObject root = doc.to<JsonObject>();

        // Top-level schema metadata
        root["$schema"] = "http://json-schema.org/draft-07/schema#";
        const char* host = !gDeviceName.empty() ? gDeviceName.c_str() : "dispenser";
        root["$id"] = String("spha://") + host + "/setup";
        root["title"] = String(host) + " Setup";
        root["type"] = "object";

        // Each service with settings becomes a nested object under properties
        JsonObject props = root.createNestedObject("properties");

        auto all = Container::getInstance()->getAllServices();
        for (IService* svc : all) {
            if (!svc || !svc->hasSettingsSchema()) continue;
            JsonObject serviceSchema = props.createNestedObject(svc->name());
            // Ask the service to populate its own object schema
            svc->buildSettingsSchema(serviceSchema);
        }

        String out;
        serializeJson(doc, out);
        _server.send(200, "application/json", out);
    });

    _server.on("/setup", HTTP_POST, [this, &_storage, _screenManager]() {
        if (!_server.hasArg("plain")) {
            _server.send(400, "application/json", "{\"error\":\"Missing body\"}");
            return;
        }
        Serial.println("HTTP /setup: body received");

        DynamicJsonDocument doc(4096);
        DeserializationError err = deserializeJson(doc, _server.arg("plain"));
        if (err) {
            _server.send(400, "application/json", "{\"error\":\"Invalid JSON\"}");
            return;
        }

        JsonObject obj = doc.as<JsonObject>();
        for (JsonObject::iterator it = obj.begin(); it != obj.end(); ++it) {
            const char* key = it->key().c_str();
            const char* value = it->value().as<const char*>();
            if (key && value) {
                _storage.setItem(key, String(value));
            }
        }

        Serial.print("HTTP /setup: wifi_ssid='");
        Serial.print(_storage.getItem("wifi_ssid", ""));
        Serial.println("' (password length hidden)");

        _server.send(200, "application/json", "{\"success\":true}");

        String newSSID = _storage.getItem("wifi_ssid", "");
        String newPass = _storage.getItem("wifi_password", "");

        delay(500);

        Serial.println("HTTP /setup: switching to STA and attempting WiFi.begin()");
        WiFi.softAPdisconnect(true);
        WiFi.mode(WIFI_STA);
        WiFi.begin(newSSID.c_str(), newPass.c_str());

        unsigned long startAttemptTime = millis();
        bool connected = false;

        while (millis() - startAttemptTime < 6000) {
            if (WiFi.status() == WL_CONNECTED) {
                connected = true;
                break;
            }
            delay(250);
        }
        Serial.printf("HTTP /setup: WiFi status after attempt: %d\n", (int)WiFi.status());

        if (connected) {
            _storage.setItem("initial_setup", "false");
            _storage.save();
            ESP.restart();
        } else {
        }

        _completed = true;
        IService* sleepTimer = Container::getInstance()->getService("SleepTimer");
        if (sleepTimer) {
        }
    });

    _server.begin();
    _lastInteraction = millis();
}

void SetupManager::loop() {
    StorageManager& storage = Container::getInstance()->getStorage();

    if (storage.getItem("initial_setup", "true") != "true") return;

    // Blink LED while BLE advertising is active
    if (bleActive) {
      unsigned long now = millis();
      if (now - lastBlink >= 500) { // toggle every 500 ms
        ledState = !ledState;
        digitalWrite(LED_BUILTIN, ledState);
        lastBlink = now;
      }
    }

    // Ensure advertising is running when not connected
    if (gServer && gServer->getConnectedCount() == 0) {
      unsigned long nowMs = millis();
      if (nowMs - lastAdvCheck > 3000) { // check every 3s
        NimBLEAdvertising* adv = NimBLEDevice::getAdvertising();
        if (adv) {
          Serial.println("BLE: watchdog re-start advertising");
          adv->start(); // safe to call if already advertising
        }
        lastAdvCheck = nowMs;
      }
    }

    _server.handleClient();

    unsigned long now = millis();

    if (_completed) return;

    IService* _sleepTimer = Container::getInstance()->getService("SleepTimer");
    if (_sleepTimer) {
        if (_clientConnected && now - _lastInteraction > 15 * 60 * 1000) {
            Serial.println("SetupManager: client timeout — going to sleep");
            _sleepTimer->execute("start");
        } else if (!_clientConnected && now - _lastInteraction > 15 * 60 * 1000) {
            Serial.println("SetupManager: no client ever connected — fallback sleep");
            _sleepTimer->execute("start");
        }
    }
}
