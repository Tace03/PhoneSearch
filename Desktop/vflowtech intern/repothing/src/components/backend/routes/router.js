import { createRouter, createWebHashHistory } from 'vue-router'
import Login from '../../interfaces/login/login.vue';
import DataRetrieve from '../../interfaces/dataRetrieve/dataRetrieve.vue';
import NotFound from '../../interfaces/errorPage/pageNotFound.vue';
import nonExistencePage from '../../interfaces/errorPage/pageNotFound.vue';
import queryData from '../../interfaces/dataRetrieve/queryData.vue';

const routes = [
    {
        path: '/login',
        name: 'login',
        component: Login,
    },
    {
        path: '/retrievedata',
        name: 'retrieveData',
        component: DataRetrieve,
    },
    {
        path: '/querydata',
        name: 'queryData',
        component: queryData,
    },
    {
        path: '/:pathMatch(.*)*',
        name: 'notfound',
        component: NotFound,
    }
]

const router = createRouter({
    history: createWebHashHistory(),
    routes
})

router.beforeEach((to, from, next) => {
    const publicPages = ['/login', '/sign-up'];
    const invalidPages = ['','/'];
    const authRequired = !publicPages.includes(to.path);
    const isInvalidPage = invalidPages.includes(to.path);
    const loggedIn = localStorage.getItem("authToken"); 
    
    if ((authRequired && !loggedIn) || isInvalidPage) {
        next('/login');
    } 
    else {
        next();
    }
});

export default router
