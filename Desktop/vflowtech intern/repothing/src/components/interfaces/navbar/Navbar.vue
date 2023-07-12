<template>
    <div class="nav-container">
        <a class="site-title">
            <div class="site-title-txt">VFlowtech</div>
        </a>
        <div class="site-logout" v-if="isLoggedIn" @click="Logout">Logout</div>
    </div>
</template>

<script>
export default {
    data() { 
        return { 
            version: dataDownloadVersion, 
            isLoggedIn: false,
        }; 
    },
    watch: {
        '$route'(to, from) {
            this.isLogin();
        }
    },
    methods: {
        Logout() {
            localStorage.removeItem("loggedUser");
            localStorage.removeItem("authToken");
            this.$router.push({ name: 'login' })
        },
        isLogin() {
            this.isLoggedIn = (localStorage.getItem('authToken') !== null)
        }
    }
};
</script>

<style scoped>
.site-title {
    float:left;
    color: rgb(238, 238, 230);
}
.site-title:hover {
    cursor: default;
    color: #555B6E;
    border-radius: 0.5rem;
    background-color: #BEE3DB;
    box-shadow: rgba(0, 0, 0, 0.25) 0px 25px 50px -12px;
}
.site-title-txt {
    font-size: 2rem;
    letter-spacing: 0.3rem;
    font-family: VarelaRound;
}
.site-logout {
    border: solid;
    cursor: pointer;
    color: #555B6E;
    font-weight: bold;
    padding: 0.2rem 1rem;
    border-width: 0.1rem;
    border-radius: 0.5rem;
    background-color: #FAF9F9;
}
.site-logout:active {
    transform: scale(0.95);
}
.nav-container {
    width: 100%;
    z-index: 999;
    display: flex;
    padding: 0 5rem;
    align-items: center;
    background-color: #555B6E;
    justify-content: space-between;
    box-shadow: rgba(0, 0, 0, 0.16) 0px 3px 6px, rgba(0, 0, 0, 0.23) 0px 3px 6px;
}

@media (max-width: 600px) {
    .nav-container {
        padding: 0;
        justify-content: center;
        gap:0.5rem;
    }
}

</style>