const { defineConfig } = require('@vue/cli-service')
const NodePolyfillPlugin = require("node-polyfill-webpack-plugin")
module.exports = defineConfig({
        configureWebpack: {
            entry: "./src/main.js",
            devServer: {
                hot: true,
                port: 8080,
                host: '0.0.0.0'
            },
            watch: true,
            watchOptions: {
                ignored: /node_modules/,
                poll: 1000,
                aggregateTimeout: 300,
            },
            plugins: [
                new NodePolyfillPlugin()
              ]
        },
        transpileDependencies: true,
    });