<template>
  <div class="flex flex-col items-center justify-center mt-12 overflow-hidden">
    <div class="relative w-52 h-52">
      <div ref="pulseOne" class="pulse-circle pulse-one"></div>
      <div ref="pulseTwo" class="pulse-circle pulse-two"></div>
    </div>
    <p class="text-lg font-medium text-gray-700 dark:text-gray-300 mt-12">{{ $t('status.scanning') }}</p>
  </div>
</template>

<script setup lang="ts">
import { ref, onMounted } from 'vue';
import { useRouter } from 'vue-router';
import { useScanBluetoothDevice } from '@/composables/scan-bluetooth-device';

// Six bright colors
const colors = ['#4469ef', '#F97316', '#2aea08', '#10B981', '#3B82F6', '#8B5CF6'];

// Template refs for the two circles
const pulseOne = ref<HTMLElement | null>(null);
const pulseTwo = ref<HTMLElement | null>(null);
const router = useRouter();
const { startScan, found, error } = useScanBluetoothDevice();

// Pick a random color
function randomColor() {
  return colors[Math.floor(Math.random() * colors.length)];
}

onMounted(async () => {
  await startScan();
  if (error.value) {
    router.push({ name: 'welcome' });
    return;
  }
  if (found.value) {
    router.push({ name: 'found' });
    return;
  }

  if (pulseOne.value) {
    // Initial color
    pulseOne.value.style.borderColor = randomColor();
    // Change color on each animation iteration
    pulseOne.value.addEventListener('animationiteration', () => {
      pulseOne.value.style.borderColor = randomColor();
    });
  }
  if (pulseTwo.value) {
    pulseTwo.value.style.borderColor = randomColor();
    pulseTwo.value.addEventListener('animationiteration', () => {
      pulseTwo.value.style.borderColor = randomColor();
    });
  }
});
</script>

<style scoped>
.pulse-circle {
  position: absolute;
  top: 0;
  left: 0;
  width: 100%;
  height: 100%;
  border: 3px solid rgba(52, 211, 153, 0.7); /* green-400 with opacity */
  border-radius: 50%;
  opacity: 0;
}

@keyframes pulse {
  0% {
    transform: scale(0);
    opacity: 0.9;
  }
  70% {
    transform: scale(1);
    opacity: 0;
  }
  100% {
    transform: scale(1);
    opacity: 0;
  }
}

.pulse-one {
  animation: pulse 2s infinite ease-out;
}

.pulse-two {
  animation: pulse 2s infinite ease-out;
  animation-delay: 1s;
}
</style>
