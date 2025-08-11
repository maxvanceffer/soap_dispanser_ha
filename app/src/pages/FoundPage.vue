<template>
  <div class="max-w-md mx-auto p-4">
    <h1 class="text-2xl font-semibold mb-6">{{ t('wifi.title') }}</h1>

    <form class="space-y-4" @submit.prevent="onSave">
      <UiInput
        id="ssid"
        v-model.trim="ssid"
        :label="t('wifi.ssid')"
        type="text"
        :placeholder="t('wifi.ssidPlaceholder')"
        autocomplete="off"
        required
      />

      <UiInput
        id="password"
        v-model="password"
        :label="t('wifi.password')"
        type="password"
        :placeholder="t('wifi.passwordPlaceholder')"
        :invalid="password && !isPassValid"
        :error-text="t('wifi.passwordTooShort', { n: MIN_PASS })"
        required
      />

      <div class="pt-2">
        <UiButton type="submit" :loading="loading" :disabled="!canSubmit || loading">
          {{ t('wifi.save') }}
        </UiButton>
      </div>

      <p v-if="!device" class="text-sm text-gray-500">{{ t('wifi.noDevice') }}</p>
    </form>
  </div>
</template>

<script setup>
import constants from '@/data/constants.js';
import { ref, computed, onMounted } from 'vue';
import { useI18n } from 'vue-i18n';
import { useStateStore } from '@/store/state.js';
import { storeToRefs } from 'pinia';
import router from '@/router';
import UiInput from '@/components/UiInput.vue';
import UiButton from '@/components/UiButton.vue';

const { t } = useI18n();
const { addMessage } = useStateStore();

const MIN_PASS = 8;

const store = useStateStore();
const { device } = storeToRefs(store);

const ssid = ref('');
const password = ref('');
const loading = ref(false);

const isPassValid = computed(() => password.value.length >= MIN_PASS);
const canSubmit = computed(() => !!ssid.value && isPassValid.value && !!device.value);

async function ensureConnected(dev) {
  console.log('[FoundPage] ensureConnected: device=', dev?.name || dev, 'connected=', !!dev?.gatt?.connected);
  if (!dev) throw new Error('No device');
  if (!dev.gatt?.connected) {
    console.log('[FoundPage] connecting to GATT...');
    await dev.gatt.connect();
    console.log('[FoundPage] GATT connected');
  }
}

async function getChars(dev) {
  await ensureConnected(dev);
  console.log('[FoundPage] getChars: using service', constants.PROV_SERVICE);
  const service = await dev.gatt.getPrimaryService(constants.PROV_SERVICE);
  console.log('[FoundPage] service acquired');
  const ssidChar = await service.getCharacteristic(constants.UUID_SSID);
  console.log('[FoundPage] characteristic SSID', constants.UUID_SSID);
  const passChar = await service.getCharacteristic(constants.UUID_PASS);
  console.log('[FoundPage] characteristic PASS', constants.UUID_PASS);
  const statusChar = await service.getCharacteristic(constants.UUID_STATUS);
  console.log('[FoundPage] characteristic STATUS', constants.UUID_STATUS);

  try {
    console.log('[FoundPage] subscribing to status notifications', constants.UUID_STATUS);
    await statusChar.startNotifications();
    const handler = (event) => {
      const val = new TextDecoder().decode(event.target.value);
      console.log('[FoundPage] status notification value =', val);
      store.provStatus = val;
      store.provInProgress = false;
    };
    statusChar.addEventListener('characteristicvaluechanged', handler);
  } catch (e) {
    console.warn('[FoundPage] Unable to start status notifications', e);
  }

  return { ssidChar, passChar, statusChar };
}

async function onSave() {
  if (!canSubmit.value) return;
  loading.value = true;
  store.provInProgress = true;
  store.provStatus = '';
  try {
    console.log('[FoundPage] onSave: sending credentials to device', device.value?.name);
    console.log('[FoundPage] SSID =', ssid.value);
    console.log('[FoundPage] PASS length =', password.value.length);
    const dev = device.value;
    const { ssidChar, passChar } = await getChars(dev);

    await ssidChar.writeValue(new TextEncoder().encode(ssid.value));
    console.log('[FoundPage] wrote SSID to', constants.UUID_SSID);
    await passChar.writeValue(new TextEncoder().encode(password.value));
    console.log('[FoundPage] wrote PASS to', constants.UUID_PASS);

    addMessage(t('wifi.savedTitle'), t('wifi.savedMessage'), 'success');
  } catch (e) {
    console.error('[FoundPage] onSave error:', e);
    console.error(e);
    store.provInProgress = false;
    addMessage(t('wifi.errorTitle'), e?.message || t('wifi.unknownError'), 'error');
  } finally {
    console.log('[FoundPage] waiting for BLE status notifications...');
    // keep button disabled until status received or small delay
    setTimeout(() => {
      loading.value = false;
    }, 1500);
  }
}

onMounted(() => {
  if (!device.value) {
    addMessage(t('wifi.noDeviceTitle'), t('wifi.noDeviceMessage'), 'error');
    console.warn('[FoundPage] no device in store; redirecting to Welcome');
    router.push({ name: 'welcome' });
  }
});
</script>
