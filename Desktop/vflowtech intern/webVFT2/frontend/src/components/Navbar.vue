<template>
    <div class="navbar">
        <h1 @click="redir">VFlowTech</h1>
        <ul class="nav_links">
        <li class="nav_link"><router-link to="/orgs" class="route">Orgs</router-link></li>
        <li class="nav_link"><router-link to="/login" class="route" @click="logOut">Log Out</router-link></li>
        </ul>
    </div>
</template>

<script>
export default {
    methods: {
        logOut() {
            this.$store.dispatch('auth/logout');
            this.$router.push('/login');
        },
        redir() {
            this.$router.push({name: 'orgs'})
        }
    }
}
</script>

<style scoped>
h1 {
    margin-right: auto;
    margin-left: 1em;
}
.navbar {
    font-size: 1.5em;
    box-sizing: border-box;
    margin: 0;
    padding: 0;
    display: flex;
    justify-content: flex-end;
    align-items: center;
    background-color: aqua;
}
.nav_links {
  list-style: none;
  display: flex;
}
.nav_link {
    padding: 0px 20px;
    display: inline-block;
    padding: 0px, 20px;
    transition: all 0.3s ease 0s;
}
.route {
    font-size: 16px;
    font-weight: 500;
    color: black;
    text-decoration: none;
}
.route:hover {
  color: blue;
}
</style>