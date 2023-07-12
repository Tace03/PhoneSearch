<template>
    <ul class="navbar-nav ml-lg-auto">
        <li class="nav-item">
            <router-link to="/orgs" class="route nav-link">Orgs</router-link>
        </li>
        <li class="nav-item">
            <router-link to="/login" class="route nav-link" @click="logOut">Log Out</router-link>
        </li>
    </ul>
</template>
<script>
export default {
    methods: {
        logOut() {
            this.$store.dispatch('auth/logout');
            this.$router.push('/login');
        }
    }
};
</script>
<style>
router-link {
    all:revert
}
</style>
