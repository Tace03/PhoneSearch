# SK_Vue_Test

This template should help get you started developing with Vue 3 in Vite.

## Recommended IDE Setup

[VSCode](https://code.visualstudio.com/) + [Volar](https://marketplace.visualstudio.com/items?itemName=Vue.volar) (and disable Vetur) + [TypeScript Vue Plugin (Volar)](https://marketplace.visualstudio.com/items?itemName=Vue.vscode-typescript-vue-plugin).

## Customize configuration

See [Vite Configuration Reference](https://vitejs.dev/config/).

## Project Setup

```sh
npm install
```

### Compile and Hot-Reload for Development

```sh
npm run dev
```

### Compile and Minify for Production

```sh
npm run build
```

### vite not found
```sh
npm install vite
```

## Push to GitPage
**1) Build**  
```sh
npm run build
```
**2) vite.config.js**  
Ensure the following line is in ```vite.config.js```.
```sh
export default defineConfig({
  // Double check "base" exist.
  base:'/vft-report-data/',
  plugins: [vue()],
})
```
**3) git add**  
```sh
git add dist -f
```
**4) git commit**  
```sh
git commit -m "message"
```
**5) git push**  
```sh
git subtree push --prefix dist origin gh-pages
```