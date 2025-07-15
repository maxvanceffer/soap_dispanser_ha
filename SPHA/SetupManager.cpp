// Updated SetupManager.cpp with safe screen replacement and spinner feedback

#include "SetupManager.h"
#include <ArduinoJson.h>
#include "./screens/SetupInfoScreen.h"

SetupManager::SetupManager(StorageManager& storage, SleepTimer& sleepTimer, ScreenManager& screenManager)
    : _storage(storage), _sleepTimer(sleepTimer), _screenManager(screenManager), _server(80), _clientConnected(false) {}

void SetupManager::begin() {
    if (_storage.getItem("initial_setup", "true") != "true") {
        Serial.println("SetupManager: Skipped (not in setup mode)");
        return;
    }

    Serial.println("SetupManager: Setup started");

    WiFi.softAP("SoapDispenser");
    Serial.println("SetupManager: SoftAP started");
    Serial.print("SetupManager: AP IP: ");
    Serial.println(WiFi.softAPIP());
    Serial.print("SetupManager: SSID: ");
    Serial.println(WiFi.softAPSSID());

    _screenManager.replaceScreen(new SetupInfoScreen({ WiFi.softAPSSID(), "http://" + WiFi.softAPIP().toString(), "Configure device in browser" }));

    _server.on("/", HTTP_GET, [this]() {
        _server.send(200, "text/html", INDEX_HTML);
        _clientConnected = true;
        _lastInteraction = millis();
    });

    _server.on("/setup", HTTP_POST, [this]() {
        if (!_server.hasArg("plain")) {
            _server.send(400, "application/json", "{\"error\":\"Missing body\"}");
            return;
        }

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

        _server.send(200, "application/json", "{\"success\":true}");

        String newSSID = _storage.getItem("wifi_ssid", "");
        String newPass = _storage.getItem("wifi_password", "");

        _screenManager.replaceScreen(new SetupInfoScreen({ "Connecting to WiFi..." }, "", 0, true));
        delay(500);

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
            _screenManager.draw(); // redraw spinner
        }

        if (connected) {
            _screenManager.replaceScreen(new SetupInfoScreen({ "WiFi OK", "Connected to:", newSSID, "Rebooting in 6s" }, "✔", 6000));
            _storage.setItem("initial_setup", "false");
            _storage.save();
            delay(6000);
            ESP.restart();
        } else {
            _screenManager.replaceScreen(new SetupInfoScreen({ "WiFi Failed", "Wrong SSID or Password" }, "!", 6000));
            delay(6000);
            _screenManager.replaceScreen(new SetupInfoScreen({ WiFi.softAPSSID(), "http://" + WiFi.softAPIP().toString(), "Configure device in browser" }));
        }

        _completed = true;
        _sleepTimer.setTimeout(_storage.getItem("sleep_timeout", "10").toInt());
        _sleepTimer.start();
    });

    _server.begin();
    _lastInteraction = millis();
}

void SetupManager::loop() {
    if (_storage.getItem("initial_setup", "true") != "true") return;

    _server.handleClient();

    unsigned long now = millis();

    if (_completed) return;

    if (_clientConnected && now - _lastInteraction > 15 * 60 * 1000) {
        Serial.println("SetupManager: client timeout — going to sleep");
        _sleepTimer.setTimeout(_storage.getItem("sleep_timeout", "10").toInt());
        _sleepTimer.start();
    } else if (!_clientConnected && now - _lastInteraction > 15 * 60 * 1000) {
        Serial.println("SetupManager: no client ever connected — fallback sleep");
        _sleepTimer.setTimeout(10);
        _sleepTimer.start();
    }
}