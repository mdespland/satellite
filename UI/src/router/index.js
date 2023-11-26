import { createRouter, createWebHistory } from 'vue-router'
import HomeView from '../views/HomeView.vue'
import ThermalView from '../views/ThermalView.vue'
import MultiSpectralView from '../views/MultiSpectralView.vue'
import MultiSpectralViewBW from '../views/MultiSpectralViewBW.vue'
import FullScreenView from '../views/FullScreenView.vue'
import SlidesFrView from '../views/SlidesFrView.vue'
import SlidesEnView from '../views/SlidesEnView.vue'
import Slide3View from '../views/Slide3View.vue'
import Slide4View from '../views/Slide4View.vue'
import SlideShowView from '../views/SlideShowView.vue'

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
      path: '/multispectralbw',
      name: 'multispectralbw',
      component: MultiSpectralViewBW
    },{
      path: '/fullscreen/:mode/:colored',
      name: 'fullscreen',
      component: FullScreenView,
      props: true
    },{
      path: '/slides/fr/4',
      name: 'slidesfr3',
      component: Slide3View
    },{
      path: '/slides/fr/5',
      name: 'slidesfr4',
      component: Slide4View
    },{
      path: '/slides/en/4',
      name: 'slidesen3',
      component: Slide3View
    },{
      path: '/slides/en/5',
      name: 'slidesen4',
      component: Slide4View
    },{
      path: '/slides/fr/:slide',
      name: 'slidesfr',
      component: SlidesFrView,
      props: true
    },{
      path: '/slides/en/:slide',
      name: 'slidesen',
      component: SlidesEnView,
      props: true
    },{
      path: '/slideshow',
      name: 'slideshow',
      component: SlideShowView
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
