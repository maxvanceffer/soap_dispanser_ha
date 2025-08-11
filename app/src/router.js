import { createRouter, createWebHistory } from 'vue-router';

const routes = [
  { path: '/', name: 'welcome', component: () => import('@/pages/WelcomePage.vue') },
  { path: '/scan', name: 'scan', component: () => import('@/pages/ScanPage.vue') },
  { path: '/found', name: 'found', component: () => import('@/pages/FoundPage.vue') },
];

const router = createRouter({
  history: createWebHistory(),
  routes,
});

export default router;
