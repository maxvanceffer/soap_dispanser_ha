import { ref, computed, onMounted, onUnmounted } from 'vue';
import { useStateStore } from '@/store/state';

const PROV_SERVICE = '12345678-1234-1234-1234-1234567890a0';
const UUID_MODEL = '12345678-1234-1234-1234-1234567890a1';
const UUID_SSID = '12345678-1234-1234-1234-1234567890a2';
const UUID_PASS = '12345678-1234-1234-1234-1234567890a3';
const UUID_STATUS = '12345678-1234-1234-1234-1234567890a4';

/**
 * Composable for scanning and provisioning a Bluetooth device.
 * Uses Pinia store to persist state across pages and ensures clean disconnects.
 */
export function useScanBluetoothDevice() {
  const store = useStateStore();

  // Local inputs
  const ssid = ref('');
  const password = ref('');
  const status = ref('');

  // Internal BLE handles (not in store to avoid serialization issues)
  let device = null;
  let server = null;
  let provSvc = null;
  let charSSID = null;
  let charPASS = null;
  let charSTATUS = null;
  let statusListener = null;

  const supportsWebBT = typeof navigator !== 'undefined' && 'bluetooth' in navigator;

  function attachDeviceListeners(dev) {
    try {
      dev.addEventListener('gattserverdisconnected', () => {
        console.debug('[useScanBluetoothDevice] gattserverdisconnected');
        store.found = false;
        store.scanning = false;
        store.messages?.push?.({ title: 'Disconnected', message: 'Bluetooth connection closed' });
      });
    } catch (_) {
      console.error(_);
    }
  }

  // Clean disconnect on page unload / navigation
  onMounted(() => {
    const cleanup = () => {
      try {
        if (device?.gatt?.connected) {
          console.debug('[useScanBluetoothDevice] disconnect on page unload');
          device.gatt.disconnect();
        }
      } catch (_) {
        console.error(_);
      }
    };
    window.addEventListener('beforeunload', cleanup);
    window.addEventListener('pagehide', cleanup);
    onUnmounted(() => {
      window.removeEventListener('beforeunload', cleanup);
      window.removeEventListener('pagehide', cleanup);
      cleanup();
    });
  });

  // Also cleanup listeners and connection if a component unmounts
  onUnmounted(() => {
    try {
      if (charSTATUS && statusListener) {
        charSTATUS.removeEventListener('characteristicvaluechanged', statusListener);
      }
    } catch (_) {
      console.error(_);
    }
    try {
      if (device?.gatt?.connected) {
        console.debug('[useScanBluetoothDevice] disconnecting device on unmount');
        device.gatt.disconnect();
      }
    } catch (_) {
      console.error(_);
    }
  });

  // Try to reconnect previously granted device (no user gesture needed)
  async function reconnectKnown() {
    if (!supportsWebBT || !navigator.bluetooth.getDevices) return false;
    try {
      const devices = await navigator.bluetooth.getDevices();
      const dev = devices.find((d) => d.name && d.name.startsWith('dispenser-'));
      if (!dev) return false;
      console.debug('[useScanBluetoothDevice] reconnectKnown found device:', dev);
      device = dev;
      attachDeviceListeners(device);
      server = await device.gatt.connect();
      provSvc = await server.getPrimaryService(PROV_SERVICE);

      const charModel = await provSvc.getCharacteristic(UUID_MODEL);
      const buf = await charModel.readValue();
      store.model = new TextDecoder().decode(buf);
      store.device = device;
      store.found = true;
      store.scanning = false;
      store.error = false;
      store.messages?.push?.({ title: 'Connected', message: 'Reconnected to device' });

      // Prepare characteristics for provisioning
      charSSID = await provSvc.getCharacteristic(UUID_SSID);
      charPASS = await provSvc.getCharacteristic(UUID_PASS);
      charSTATUS = await provSvc.getCharacteristic(UUID_STATUS);
      statusListener = (event) => {
        status.value = new TextDecoder().decode(event.target.value);
        store.scanning = false;
      };
      await charSTATUS.startNotifications();
      charSTATUS.addEventListener('characteristicvaluechanged', statusListener);

      return true;
    } catch (e) {
      console.debug('[useScanBluetoothDevice] reconnectKnown error:', e);
      return false;
    }
  }

  // Start BLE scan and read model
  async function startScan() {
    console.debug('[useScanBluetoothDevice] startScan() called');
    if (!supportsWebBT) {
      store.error = true;
      store.messages?.push?.({ title: 'Scan Error', message: 'Web Bluetooth not supported in this browser.' });
      return false;
    }
    store.scanning = true;
    store.error = false;
    store.found = false;
    status.value = '';
    store.model = '';
    try {
      console.debug('[useScanBluetoothDevice] requesting Bluetooth device with filters:', {
        filters: [{ namePrefix: 'dispenser-' }],
        optionalServices: [PROV_SERVICE],
      });
      device = await navigator.bluetooth.requestDevice({
        filters: [{ namePrefix: 'dispenser-' }],
        optionalServices: [PROV_SERVICE],
      });
      console.debug('[useScanBluetoothDevice] device selected:', device);
      store.device = device;
      attachDeviceListeners(device);

      server = await device.gatt.connect();
      console.debug('[useScanBluetoothDevice] GATT server connected');
      provSvc = await server.getPrimaryService(PROV_SERVICE);

      // Read model characteristic
      const charModel = await provSvc.getCharacteristic(UUID_MODEL);
      const buf = await charModel.readValue();
      store.model = new TextDecoder().decode(buf);
      store.found = true;
      console.debug('[useScanBluetoothDevice] device model read:', store.model);
      console.log('Подключено устройство. Модель:', store.model);

      // Prepare SSID, PASS, STATUS characteristics
      charSSID = await provSvc.getCharacteristic(UUID_SSID);
      charPASS = await provSvc.getCharacteristic(UUID_PASS);
      charSTATUS = await provSvc.getCharacteristic(UUID_STATUS);

      // Listen for status notifications (OK/ERROR)
      statusListener = (event) => {
        status.value = new TextDecoder().decode(event.target.value);
        store.scanning = false;
      };
      await charSTATUS.startNotifications();
      charSTATUS.addEventListener('characteristicvaluechanged', statusListener);
      console.debug('[useScanBluetoothDevice] status notifications started');

      store.messages?.push?.({ title: 'Connected', message: `Device ${store.model || ''} connected` });
      return true;
    } catch (e) {
      console.debug('[useScanBluetoothDevice] startScan error:', e);
      store.error = true;
      store.scanning = false;
      store.messages?.push?.({ title: 'Scan Error', message: e.message || String(e) });
      return false;
    } finally {
      store.scanning = false;
    }
  }

  // Write SSID and password to the device
  async function provisionDevice() {
    console.debug('[useScanBluetoothDevice] provisionDevice() called with SSID:', ssid.value);
    if (!charSSID || !charPASS) {
      console.warn('[useScanBluetoothDevice] SSID or PASS characteristic not initialized');
      store.messages?.push?.({ title: 'Provision', message: 'Device is not ready for provisioning yet.' });
      return false;
    }
    try {
      await charSSID.writeValue(new TextEncoder().encode(ssid.value));
      await charPASS.writeValue(new TextEncoder().encode(password.value));
      console.debug('[useScanBluetoothDevice] SSID and password written');
      store.messages?.push?.({ title: 'Provision', message: 'Wi‑Fi credentials sent to device' });
      // status will update via notification listener
      return true;
    } catch (e) {
      console.debug('[useScanBluetoothDevice] provisionDevice error:', e);
      status.value = e.message || String(e);
      store.scanning = false;
      store.messages?.push?.({ title: 'Provision Error', message: e.message || String(e) });
      return false;
    }
  }

  function disconnect() {
    try {
      if (device?.gatt?.connected) {
        device.gatt.disconnect();
        console.debug('[useScanBluetoothDevice] manual disconnect');
        store.messages?.push?.({ title: 'Disconnected', message: 'Bluetooth connection closed' });
      }
    } catch (e) {
      console.debug('[useScanBluetoothDevice] disconnect error:', e);
    } finally {
      store.found = false;
      store.scanning = false;
    }
  }

  const loadingText = computed(() => (store.scanning ? 'Scanning...' : ''));
  const statusText = computed(() => status.value);

  return {
    model: computed(() => store.model),
    ssid,
    password,
    status,
    scanning: computed(() => store.scanning),
    found: computed(() => store.found),
    error: computed(() => store.error),
    loadingText,
    statusText,
    startScan,
    reconnectKnown,
    disconnect,
    provisionDevice,
  };
}
