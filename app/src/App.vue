<template>
  <div
    :class="[
      'min-h-screen pt-8 transition-colors',
      theme === 'dark' ? 'bg-gray-900 text-gray-100' : 'bg-white text-gray-900',
    ]"
  >
    <div class="max-w-md mx-auto font-sans p-4">
      <header
        class="bg-gradient-to-r from-blue-600 to-purple-600 text-white rounded-lg p-4 shadow-lg flex items-center justify-between"
      >
        <div class="flex items-center gap-2">
          <Globe class="w-5 h-5" />
          <select
            v-model="curLocale"
            class="bg-white bg-opacity-20 text-white text-sm rounded-md border-none focus:ring-0 px-3 py-1"
          >
            <option v-for="code in locales" :key="code" :value="code">{{ t(`langName.${code}`) }}</option>
          </select>
        </div>
        <button class="p-2 rounded-full bg-white bg-opacity-20 hover:bg-opacity-30 transition" @click="toggleTheme">
          <component :is="theme === 'dark' ? Sun : Moon" class="w-5 h-5" />
        </button>
      </header>
      <device-card />
      <main>
        <RouterView />
      </main>
      <!-- Toast notification -->
      <UiToast v-if="currentToast" :title="currentToast.title" :message="currentToast.message" />
    </div>
  </div>
</template>

<script setup>
import { ref, watch } from 'vue';
import UiToast from './components/UiToast.vue';
import DeviceCard from './components/DeviceCard.vue';
import { useStateStore } from './store/state';
import { computed, inject } from 'vue';
import { useI18n } from 'vue-i18n';
import { supportedLocales, setLocale } from './i18n';
import { toggleTheme } from './theme';
import { Sun, Moon, Globe } from 'lucide-vue-next';

const { t, locale } = useI18n();
const locales = supportedLocales;
const curLocale = computed({
  get: () => locale.value,
  set: (v) => setLocale(v),
});
const theme = inject('theme');

const store = useStateStore();
const currentToast = ref(null);

// Watch for new messages in the store and display them one at a time
watch(
  () => store.messages,
  (msgs) => {
    if (!currentToast.value && msgs.length > 0) {
      // Take the first message from the store
      currentToast.value = msgs.shift();
      // Hide toast after 3 seconds
      setTimeout(() => {
        currentToast.value = null;
      }, 3000);
    }
  },
);
</script>
