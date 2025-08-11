import { createI18n } from 'vue-i18n';
import en from './i18n/locales/en.js';

const SUPPORTED = ['en', 'ru', 'ro', 'it', 'fr'];

function detectLocale() {
  const saved = localStorage.getItem('locale');
  if (saved && SUPPORTED.includes(saved)) return saved;
  const candidates = navigator.languages?.length ? navigator.languages : [navigator.language];
  for (const l of candidates) {
    const base = l.split('-')[0].toLowerCase();
    if (SUPPORTED.includes(base)) return base;
  }
  return 'en';
}

// Create i18n with only English preloaded (default)
export const i18n = createI18n({
  legacy: false,
  locale: 'en',
  fallbackLocale: 'en',
  messages: { en },
});

export const supportedLocales = SUPPORTED;

const loaded = new Set(['en']);

async function loadLocaleMessages(locale) {
  if (loaded.has(locale)) return;
  let msgs;
  if (locale === 'ru') {
    msgs = (await import('./i18n/locales/ru.js')).default;
  } else if (locale === 'ro') {
    msgs = (await import('./i18n/locales/ro.js')).default;
  } else if (locale === 'it') {
    msgs = (await import('./i18n/locales/it.js')).default;
  } else if (locale === 'fr') {
    msgs = (await import('./i18n/locales/fr.js')).default;
  } else if (locale === 'en') {
    msgs = en;
  }
  if (msgs) {
    i18n.global.setLocaleMessage(locale, msgs);
    loaded.add(locale);
  }
}

export async function setLocale(locale) {
  if (!SUPPORTED.includes(locale)) return false;
  await loadLocaleMessages(locale);
  i18n.global.locale.value = locale;
  localStorage.setItem('locale', locale);
  return true;
}

// Auto-apply detected locale on startup (lazy-load if not EN)
const initial = detectLocale();
if (initial !== 'en') {
  // Fire and forget; UI will update once loaded
  setLocale(initial);
}
