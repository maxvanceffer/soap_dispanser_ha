<template>
  <div class="w-full">
    <label
      v-if="label"
      :for="id"
      class="block text-sm font-medium mb-1"
      :class="isDark ? 'text-gray-200' : 'text-gray-700'"
    >
      {{ label }}
    </label>
    <div class="relative">
      <input
        :id="id"
        :type="type"
        class="w-full rounded-md border px-3 py-2 focus:outline-none focus:ring-2 transition-colors"
        :class="[
          // theme-aware background, text, placeholder, border
          isDark
            ? 'bg-gray-800 text-gray-100 placeholder-gray-400 border-gray-700 focus:ring-blue-500'
            : 'bg-white text-gray-900 placeholder-gray-500 border-gray-300 focus:ring-blue-500',
          // validation ring/border
          invalid ? 'border-red-500 focus:ring-red-500' : '',
          // disabled
          disabled ? 'opacity-50 cursor-not-allowed' : '',
        ]"
        :placeholder="placeholder"
        :value="modelValue"
        :disabled="disabled"
        :autocomplete="autocomplete"
        :required="required"
        @input="$emit('update:modelValue', $event.target.value)"
      />
      <slot name="right" />
    </div>
    <p v-if="hint" class="mt-1 text-xs" :class="isDark ? 'text-gray-400' : 'text-gray-500'">{{ hint }}</p>
    <p v-if="invalid && errorText" class="mt-1 text-xs" :class="isDark ? 'text-red-400' : 'text-red-600'">
      {{ errorText }}
    </p>
  </div>
</template>

<script setup>
import { inject, computed } from 'vue';

defineEmits(['update:modelValue']);

defineProps({
  modelValue: { type: [String, Number], default: '' },
  label: { type: String, default: '' },
  id: { type: String, default: '' },
  type: { type: String, default: 'text' },
  placeholder: { type: String, default: '' },
  disabled: Boolean,
  required: Boolean,
  invalid: Boolean,
  errorText: { type: String, default: '' },
  hint: { type: String, default: '' },
  autocomplete: { type: String, default: 'off' },
});

const theme = inject('theme');
const isDark = computed(() => theme?.value === 'dark');
</script>
