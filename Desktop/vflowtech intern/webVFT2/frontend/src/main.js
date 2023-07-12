import { createApp } from 'vue'
import App from './App.vue'
import router from './router'
import store from "./store";
import Argon from './plugins/argon-kit.js'
import VueApexCharts from "vue3-apexcharts";
import { createVuetify } from 'vuetify';
import * as components from 'vuetify/components'
import * as directives from 'vuetify/directives'

const vuetify = createVuetify({
  components,
  directives,
})

// import './index.css'

createApp(App).use(router).use(Argon).use(VueApexCharts).use(vuetify).use(store).mount('#app')