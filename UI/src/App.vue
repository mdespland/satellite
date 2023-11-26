<script setup>
import { ref, onMounted, onUnmounted } from 'vue'
import { RouterLink, RouterView } from 'vue-router'
import { useRouter, useRoute } from 'vue-router'

import { inject } from 'vue'
const page = inject('page')
const language = inject('language')
const slide = inject('slide')
const slideauto = inject('slideauto')

import Satellite from './components/Satellite.vue'

const router = useRouter()

onMounted(() => {
  window.addEventListener('keydown', doCommand);
});

onUnmounted(() => {
  window.removeEventListener('keydown', doCommand);
});

function doCommand(e) {

  let cmd = String.fromCharCode(e.keyCode).toLowerCase();
  if (cmd==="(") {
    //next
    switch (page.value) {
      case 'hyperspectral':
        router.push("/multispectralbw")
      break;
      case 'slides':
        slide.value++;
        if (slide.value>12) slide.value=0;
        console.log("push /slides/"+language.value+"/"+slide.value)
        router.push("/slides/"+language.value+"/"+slide.value)
      break;
    }
  } else {
    //back
    switch (page.value) {
      case 'hyperspectralbw':
        router.push("/multispectral")
      break;
      case 'slides':
        slide.value--;
        if (slide.value<0) slide.value=0;
        router.push("/slides/"+language.value+"/"+slide.value)
      break;
    }
  }
  // do stuff
  console.log(cmd);
  console.log("Page : "+page.value+" "+language.value+" slide : "+slide.value)

}

</script>

<template>
  <div class="home"><RouterLink to="/" class="routerlink"><img class="logo" alt="Vue logo" src="@/assets/orange-logo.png" /></RouterLink></div>
  <RouterView class="routerview"/>
</template>

<style scoped>
header {
  margin: 0;
  max-height: 140px;
  width:100vw;
  height:140px;
}

.logo {
  display: block;
  margin: 20px;
  float: right;
  height:  100px;
}


/*.home {
  margin: 20px;
  right:0;
  bottom:0;
  display: block;
  float: right;
  position: absolute;
  z-index: 10;
}*/

.home,
.routerview {
  width: 100vw;
  max-height: 100vh;
  height:100vh;
  position: absolute;
  
}
 

.routerview {
  z-index: -1;
}
.home {
  z-index: 10;
  pointer-events: none; 

}
.routerlink {
  pointer-events: auto; 

}
.logo {
  margin: 20px;
  right:0;
  bottom:0;
  display: block;
  float: right;
  position: absolute;
  z-index: 10;  
}
</style>
