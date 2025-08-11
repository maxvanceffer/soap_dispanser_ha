<template>
  <div class="flex flex-col items-center justify-center">
    <div class="bg-white dark:bg-gray-900 rounded-xl p-8 max-w-sm w-full text-center">
      <h1 class="text-3xl font-bold mb-4 text-gray-800 dark:text-gray-100">
        {{ t('welcomeMessage') }}
      </h1>
      <p class="text-gray-600 dark:text-gray-300 mb-6">
        {{ t('welcomeDescription') }}
      </p>
      <button
        class="w-full py-3 bg-gradient-to-r from-blue-600 to-purple-600 text-white font-semibold rounded-lg hover:from-blue-700 hover:to-purple-700 transition"
        @click="startScan"
      >
        {{ t('startScan') }}
      </button>
    </div>
  </div>
</template>

<script setup lang="ts">
import { useI18n } from 'vue-i18n';
import { useRouter } from 'vue-router';

const { t } = useI18n();
const router = useRouter();

function startScan() {
  router.push({ name: 'scan' });
}
</script>

<style scoped>
/* No additional styles; all styling via Tailwind */
</style>
