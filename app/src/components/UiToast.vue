<template>
  <div class="fixed bottom-0 left-0 w-full px-4 py-2 md:bottom-4 md:right-4 md:left-auto md:w-auto">
    <div class="bg-white dark:bg-gray-800 shadow-lg rounded-lg p-4 max-w-md mx-auto md:mx-0">
      <h3 class="text-lg font-semibold text-gray-900 dark:text-gray-100">
        {{ title }}
      </h3>
      <p class="mt-1 text-sm text-gray-700 dark:text-gray-300">
        {{ message }}
      </p>
    </div>
  </div>
</template>

<script setup>
defineProps({
  title: {
    type: String,
    default: '',
    required: true,
  },
  message: {
    type: String,
    default: '',
    required: true,
  },
});
</script>
