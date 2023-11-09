//import './assets/main.css'
import { createApp } from 'vue'
import App from './App.vue'
import router from './router'

const app = createApp(App)
app.config.globalProperties.page="demo"
app.config.globalProperties.slide=0
app.use(router)

app.mount('#app')
