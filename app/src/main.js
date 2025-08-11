import { createApp } from 'vue';
import { createPinia } from 'pinia';
import App from './App.vue';
import './assets/main.css';
import { i18n } from './i18n';
import { theme } from './theme';
import router from './router';

const pinia = createPinia();
const app = createApp(App);
app.use(i18n);
app.use(router);
app.use(pinia);
app.provide('theme', theme);
app.mount('#app');
