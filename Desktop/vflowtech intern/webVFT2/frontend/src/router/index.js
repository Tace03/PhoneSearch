import { createRouter, createWebHistory } from 'vue-router'
import Login from '../views/Login.vue'
import Orgs from '../views/Orgs.vue'
import Signup from '../views/Signup.vue'
import DashboardList from '../views/DashboardList.vue'
import Dashboard from '../views/Dashboard.vue'
import NotFound from '../views/NotFound.vue'

const routes = [
  {
    path: '/login',
    name: 'login',
    component: Login,
    meta: {
        showNavbar: false
    }
  },
  {
    path: '/orgs',
    name: 'orgs',
    component: Orgs
  },
  {
    path: '/orgs/:id',
    name: 'dashList',
    component: DashboardList
  },
  {
    path: '/orgs/:org_id/dashboard/:dash_id',
    name: 'dashboard',
    component: Dashboard
  },
  {
    path: '/',
    redirect: '/login'
  },
  {
    path: '/sign-up',
    name: 'signup',
    component: Signup
  },
  {
    path: '/:pathMatch(.*)*',
    name: 'notfound',
    component: NotFound
  }
]

const router = createRouter({
  history: createWebHistory(process.env.BASE_URL),
  routes
})

router.beforeEach((to, from, next) => {
    const publicPages = ['/login', '/sign-up'];
    const authRequired = !publicPages.includes(to.path);
    const loggedIn = sessionStorage.getItem('user');
    // trying to access a restricted page + not logged in
    // redirect to login page
    if (authRequired && !loggedIn) {
      next('/login');
    } else {
      next();
    }
  });

export default router
