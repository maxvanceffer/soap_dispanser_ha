import { ref, watch } from 'vue';

const THEME_KEY = 'theme';
export const theme = ref(
  localStorage.getItem(THEME_KEY) || (window.matchMedia('(prefers-color-scheme: dark)').matches ? 'dark' : 'light'),
);

// Apply theme class on <html>
function apply(t) {
  const cls = document.documentElement.classList;
  t === 'dark' ? cls.add('dark') : cls.remove('dark');
}
apply(theme.value);

// Watch and save
watch(theme, (val) => {
  localStorage.setItem(THEME_KEY, val);
  apply(val);
});

export function toggleTheme() {
  theme.value = theme.value === 'dark' ? 'light' : 'dark';
}
