<template>
    <div class="my-4">
        <button @click="this.$router.push({ name: 'orgs' })">Back</button>
    </div>
    <table v-if="resources">
        <tr>
            <td>Dashboard Name</td>
            <td>Shared by:</td>
            <td>Shared on:</td>
        </tr>
        <tr v-for="resource in resources" :key="resource.resource.id">
            <td @click="redir(resource.resource.id)">{{ resource.resource.name }}</td>
            <td @click="redir(resource.resource.id)">{{ `${resource.share_by.first_name} ${resource.share_by.last_name}` }}</td>
            <td @click="redir(resource.resource.id)">{{ resource.shared_at }}</td>
        </tr>
    </table>
    <p v-else>Loading...</p>
</template>

<script>
import FetchService from '../services/fetch-service';
export default {
    data() {
        return {
            resources: null
        }
    },
    methods: {
        async getData() {
            const res = await FetchService.fetch_dash_list(this.$route.params.id);
            this.resources = res.resources;
        },
        redir(id) {
            this.$router.push({name: "dashboard", params: {org_id: this.$route.params.id, dash_id: id}});
        }
    },
    mounted() {
        this.getData();
    }
}
</script>

<style scoped>
table, th, td {
  border: 1px solid;
}
table {
    margin: 2rem;
    border-collapse: collapse;
    width: 80%;
    align-items: center;
}
button {
    display: block; /* Change display to block to take full width */
    margin-right: auto; /* Set the right margin to auto */
    margin-left: 2rem; /* Set the left margin to 0 */
}
</style>