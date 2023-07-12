import App from './App.vue';
import { createApp } from 'vue';

// Vue Router
import router from './components/backend/routes/router';

// External Libraries
import "bootstrap";
import '@fortawesome/vue-fontawesome';
import * as Sentry from "@sentry/vue";
import VueExcelXlsx from "vue-excel-xlsx";
import { BNavbar, BootstrapVue3 } from "bootstrap-vue-3";
import VueCtkDateTimePicker from 'vue-ctk-date-time-picker';
import 'vue-ctk-date-time-picker/dist/vue-ctk-date-time-picker.css';

// External CSS
import './assets/css/main.css'
import './assets/css/navbar.css'

// Create App before declaring Sentry
const app = createApp(App);
Sentry.init({
    app,
    dsn: "https://b568267d0ebb4d53b189dcb483443218@o4505362450546688.ingest.sentry.io/4505385172598784",
    integrations: [
        new Sentry.BrowserTracing({
            logErrors: true,
            release: __SENTRY_RELEASE__,
            environment: import.meta.env.MODE,
            tracePropagationTargets: ["localhost", /^https:\/\/yourserver\.io\/api/],
            routingInstrumentation: Sentry.vueRouterInstrumentation(router),
        }),
        // If replay of what user clicked is required
        // new Sentry.Replay(),
    ],
    // Performance Monitoring
    tracesSampleRate: 1.0,
    // Set sampling rate for profiling - this is relative to tracesSampleRate
    profilesSampleRate: 1.0,
    // Session Replay
    //replaysSessionSampleRate: 0.1, // This sets the sample rate at 10%. You may want to change it to 100% while in development and then sample at a lower rate in production.
    //replaysOnErrorSampleRate: 1.0, // If you're not already sampling the entire session, change the sample rate to 100% when sampling sessions where errors occur.
});

app.use(router)
app.use(VueExcelXlsx);
app.use(BootstrapVue3);
app.component('VueCtkDateTimePicker', VueCtkDateTimePicker);
app.mount('#app');