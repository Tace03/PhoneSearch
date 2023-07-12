import { createStore } from "vuex";
import { auth } from "./auth-module";

export const store = createStore({
  state: {
    isNavOpen: false
  },
  mutations: {
    setIsNavOpen(state, yesno) {
      state.isNavOpen = yesno;
    },
    toggle(state) {
      state.isNavOpen = !state.isNavOpen;
    }
  },
  actions: {
    toggleNav(context) {
        const isNavOpen = context.state.isNavOpen;
        context.commit('toggle', isNavOpen);
    }
  },
  modules: {
    auth,
  }
});

export default store;