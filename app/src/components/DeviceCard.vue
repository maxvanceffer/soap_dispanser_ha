<template>
  <div v-if="connected" class="mt-4">
    <div
      class="flex items-center gap-4 p-4 rounded-lg shadow-lg border transition-colors"
      :class="isDark ? 'bg-gray-800 border-gray-700 text-gray-100' : 'bg-white border-gray-200 text-gray-900'"
    >
      <div class="w-16 h-16 rounded-lg flex items-center justify-center">
        <img v-if="imageSrc" :src="imageSrc" alt="device" class="w-16 h-16 object-contain" />
        <Wifi v-else class="w-8 h-8 text-blue-500" />
      </div>
      <div class="flex-1 min-w-0">
        <div class="flex items-center gap-2">
          <h3 class="text-lg font-semibold truncate">{{ deviceInfo?.name || model }}</h3>
          <span
            v-if="deviceInfo?.brand"
            class="text-xs px-2 py-0.5 rounded-full"
            :class="isDark ? 'bg-gray-700 text-gray-200' : 'bg-gray-200 text-gray-700'"
          >
            {{ deviceInfo.brand }}
          </span>
        </div>
        <p class="text-xs opacity-70">{{ deviceInfo?.manufacturer || '—' }}</p>
        <div v-if="showWifi" class="mt-2 flex items-center gap-2">
          <component :is="wifiIcon" class="w-4 h-4" :class="wifiIconClass" />
          <span class="text-sm" :class="wifiTextClass">{{ wifiText }}</span>
        </div>
      </div>
    </div>
  </div>
</template>

<script setup>
import { computed, inject } from 'vue';
import { useStateStore } from '@/store/state';
import devices from '@/data/devices';
import { Wifi, WifiOff, Loader2 } from 'lucide-vue-next';

const theme = inject('theme');
const isDark = computed(() => theme?.value === 'dark');

const store = useStateStore();
const connected = computed(() => !!store.device);
const model = computed(() => store.model);

const deviceInfo = computed(() => devices.find((d) => d.id === model.value));
const imageSrc = computed(() => {
  const img = deviceInfo.value?.image;
  if (!img) return '';
  try {
    return new URL(`../assets/devices/${img}`, import.meta.url).href;
  } catch (e) {
    return '';
  }
});

const showWifi = computed(() => store.provInProgress || !!store.provStatus);

const wifiIcon = computed(() => {
  if (store.provInProgress) return Loader2;
  if (store.provStatus) return store.provStatus === 'OK' ? Wifi : WifiOff;
  return Wifi;
});
const wifiIconClass = computed(() => {
  if (store.provInProgress) return 'text-blue-500 animate-spin';
  if (store.provStatus) return store.provStatus === 'OK' ? 'text-green-500' : 'text-red-500';
  return '';
});
const wifiText = computed(() => {
  if (store.provInProgress) return 'Connecting to Wi‑Fi...';
  if (store.provStatus) return store.provStatus === 'OK' ? 'Connected' : 'Error connecting';
  return '';
});
const wifiTextClass = computed(() => {
  if (store.provInProgress) return isDark.value ? 'text-blue-300' : 'text-blue-600';
  if (store.provStatus)
    return store.provStatus === 'OK'
      ? isDark.value
        ? 'text-green-300'
        : 'text-green-600'
      : isDark.value
        ? 'text-red-300'
        : 'text-red-600';
  return '';
});
</script>
