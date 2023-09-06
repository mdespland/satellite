import { createRouter, createWebHistory } from 'vue-router'
import HomeView from '../views/HomeView.vue'
import ThermalView from '../views/ThermalView.vue'
import MultiSpectralView from '../views/MultiSpectralView.vue'
import FullScreenView from '../views/FullScreenView.vue'

const router = createRouter({
  history: createWebHistory(import.meta.env.BASE_URL),
  routes: [
    {
      path: '/',
      name: 'home',
      component: HomeView
    },
    {
      path: '/thermal',
      name: 'thermal',
      component: ThermalView
    },{
      path: '/multispectral',
      name: 'multispectral',
      component: MultiSpectralView
    },{
      path: '/fullscreen/:mode/:colored',
      name: 'fullscreen',
      component: FullScreenView,
      props: true
    },{
      path: '/about',
      name: 'about',
      // route level code-splitting
      // this generates a separate chunk (About.[hash].js) for this route
      // which is lazy-loaded when the route is visited.
      component: () => import('../views/AboutView.vue')
    }
  ]
})

export default router
