import { fileURLToPath, URL } from 'node:url';

import { defineConfig } from 'vite';
import vue from '@vitejs/plugin-vue';
import viteSentry from 'vite-plugin-sentry';

const path = require("path");
require("dotenv").config({ path: path.resolve(__dirname, '.env.local') });

const viteSentryConfig = {
    authToken: "05ddd3116b2443cb91c881caa302f3edeb6291bc25de426cae5f81e1891071eb", //process.env.SENTRY_AUTH_TOKEN,
    org: "vflowtech",
    project: "vflowtech-deployment-project",
    release: `${process.env.npm_package_name}@${process.env.npm_package_version}`,
    setCommits: {
        auto: true,
    },
    sourceMaps: {
        include: ["./dist/assets"],
        ignore: ["node_modules"],
        urlPrefix: "~/assets",
    },
};

export default defineConfig({
    // base:'/vft-report-data/',
    base: '/',
    plugins: [vue(), viteSentry(viteSentryConfig)],
    resolve: {
        alias: {
            '@': fileURLToPath(new URL('./src', import.meta.url))
        }
    },
    define: {
        __SENTRY_RELEASE__: `"${process.env.npm_package_name}@${process.env.npm_package_version}"`,
        dataDownloadVersion: `"${process.env.npm_package_version}"`
    },
    // ===== This is commented for netlify ====== //
    // build:{ 
    //   sourcemap: "hidden",
    // },
    // ========================================== //
})