<template>
    <div class="row row-cols-1 row-cols-md-3">
        <div class="card text-center w-75 col mb-4 px-auto py-auto" v-for="org in organizations" :key="org.id">
            <div class="card-body">
                <img :src="org.avatar" @click="redir(org.id)" />
                <p>{{org.name}}</p>
            </div>
        </div>
    </div>
</template>

<script>
export default {
    data() {
        return {
            organizations: JSON.parse(sessionStorage.getItem('user')).organisations
        }
    },
    methods: {
        redir(id) {
            this.$emit('orgSelected', id);
        }
    }
}
</script>

<style scoped>
ul {
    display: flex;
    align-items: center;
    justify-content: center;
}
img {
    height: 200;
    width: 300px;
}
li {
    display:inline-block;
    margin: 2rem;
}
</style>