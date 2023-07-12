<template>
    <div class="nav-container">
        <div class = "site-title-version">© 2023 VFlowTech Intern - Version {{ version }}</div>
    </div>
</template>

<script>
export default {
  data() { return { version: dataDownloadVersion, }; },
};
</script>

<style scoped>
.nav-container {
    width: 100%;
    z-index: 999;
    display: flex;
    padding: 0 5rem;
    align-items: center;
    background-color: #555B6E;
    justify-content: center;
    text-align: center;
    box-shadow: rgba(0, 0, 0, 0.16) 0px 3px 6px, rgba(0, 0, 0, 0.23) 0px 3px 6px;
}
.site-title-version {
    color: rgb(238, 238, 230);
}
</style>