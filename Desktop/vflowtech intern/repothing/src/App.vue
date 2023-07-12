<script setup>
import Navbar from './components/interfaces/navbar/Navbar.vue'
import Footer from './components/interfaces/navbar/Footer.vue'
</script>

<template>
    <div class="mainContainer">
        <Navbar />
        <main class="ContentContainer">
            <RouterView />
        </main>
        <Footer />
    </div>
</template>

<script>
export default {
    components: [Navbar],
}
</script>

<style scoped>
.ContentContainer {
    background-color: #89B0AE;
    background-image: url("@/assets/cubes.png");
}
</style>