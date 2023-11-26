//import './assets/main.css'
import { createApp } from 'vue'
import App from './App.vue'
import router from './router'
import { ref, provide } from 'vue'

const app = createApp(App)
const page=ref("init")
const slide=ref(0)
const language=ref("fr")
const slideauto=ref(0)
app.provide('page', page)
app.provide('slide', slide)
app.provide('language', language)
app.provide('slideauto', slideauto)
app.config.globalProperties.page="demo"
app.config.globalProperties.slide=0
app.use(router)

app.mount('#app')
