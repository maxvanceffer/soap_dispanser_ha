<template>
  <button
    :type="type"
    class="inline-flex items-center justify-center rounded-md px-4 py-2 font-medium transition-colors focus:outline-none focus:ring-2"
    :class="[
      color === 'primary' ? 'bg-blue-600 text-white hover:bg-blue-700 focus:ring-blue-500' : '',
      color === 'secondary' ? 'bg-gray-200 text-gray-900 hover:bg-gray-300 focus:ring-gray-400' : '',
      disabled || loading ? 'opacity-50 cursor-not-allowed' : '',
    ]"
    :disabled="disabled || loading"
  >
    <svg
      v-if="loading"
      class="animate-spin -ml-1 mr-2 h-5 w-5"
      xmlns="http://www.w3.org/2000/svg"
      viewBox="0 0 24 24"
    >
      <circle class="opacity-25" cx="12" cy="12" r="10" stroke="currentColor" stroke-width="4" />
      <path class="opacity-75" fill="currentColor" d="M4 12a8 8 0 018-8v4a4 4 0 00-4 4H4z" />
    </svg>
    <slot />
  </button>
</template>

<script setup>
defineProps({
  type: { type: String, default: 'button' },
  color: { type: String, default: 'primary' },
  disabled: Boolean,
  loading: Boolean,
});
</script>
