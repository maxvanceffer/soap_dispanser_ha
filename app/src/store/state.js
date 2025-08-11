import { defineStore } from 'pinia';

// You can name the return value of `defineStore()` anything you want,
// but it's best to use the name of the store and surround it with `use`
// and `Store` (e.g. `useUserStore`, `useCartStore`, `useProductStore`)
// the first argument is a unique id of the store across your application
export const useStateStore = defineStore('state', {
  state: () => ({
    device: null,
    model: null,
    scanning: false,
    found: false,
    error: false,
    provStatus: '',
    provError: '',
    provInProgress: false,
    messages: [],
  }),
  actions: {
    addMessage(title, message, type = 'info') {
      const object = { id: Date.now(), title, message, type };
      // Store full message object for UiToast
      this.messages.push(object);
      return object.id;
    },
    clearMessages() {
      this.messages = [];
    },
  },
});
