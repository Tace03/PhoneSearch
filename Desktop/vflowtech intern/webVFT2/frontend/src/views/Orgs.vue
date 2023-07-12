<template>
    <div class="container">
        <Organizations @orgSelected="redirDashList" v-if="showOrgs"/>
    </div>
</template>

<script>
import Sidebar from '../components/Sidebar.vue';
import Organizations from '../components/Organizations.vue';

export default {
  name: 'Home',
  components: {
    Sidebar,
    Organizations
  },
  data()  {
    return {
        dash_id: null,
        showOrgs: true,
        dashList: null
    }
  },
  computed: {
    currentUser() {
      return this.$store.state.auth.user;
    }
  },
  mounted() {
    if (!this.currentUser) {
      this.$router.push('/login');
    }
  },
  methods: {
    redirDashList(id) {
        this.$router.push({name: 'dashList', params: {id: id}});
    }
  }
}
</script>
<style>
</style>
