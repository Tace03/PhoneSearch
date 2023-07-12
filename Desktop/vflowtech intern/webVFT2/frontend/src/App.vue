<template>
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <base-nav v-if="showNavbar" v-for="menu in menus"
                  :key="menu.type"
                  :type="menu.type"
                  effect="dark"
                  expand
                  :content-id="`navbar-${menu.type}`">
            <div class="row" slot="content-header">
                <div class="col-6 collapse-brand">
                    <img class="navbar-brand" width="170" height="80" @click="redir" src="./assets/vftlogo.png">
                </div>
            </div>
            <component :is="menu.menuComponent"></component>
    </base-nav>
    <router-view class="flex flex-col min-h-screen"/>
</template>

<script>
import BaseNav from "@/components/BaseNav";
import Menu2 from "./views/Menu2";
export default {
    components: {
        BaseNav
    },
    methods: {
        redir() {
            this.$router.push({name: 'orgs'})
        }
    },
    data() {
    return {
      menus: [
        { type: "primary", menuComponent: Menu2 }
      ]
    };
    },
    computed: {
        showNavbar() {
        // Check the meta field of the current route
        const currentRoute = this.$route;
        return currentRoute.meta.showNavbar !== false;
        }
    }
}
</script>

<style>
#app {
  font-family: Avenir, Helvetica, Arial, sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  text-align: center;
  color: #2c3e50;
  margin: 0;
}
body {
    margin: 0 !important;
}
.flex {
  display: flex;
}

.flex-col {
  flex-direction: column;
}

.min-h-screen {
  min-height: 100vh;
}
</style>
