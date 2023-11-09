<script setup>
import { ref, onMounted, onUnmounted } from 'vue'
import { RouterLink, RouterView } from 'vue-router'
import { useRouter, useRoute } from 'vue-router'

import Satellite from './components/Satellite.vue'

const router = useRouter()

onMounted(() => {
  window.addEventListener('keydown', doCommand);
});

onUnmounted(() => {
  window.removeEventListener('keydown', doCommand);
});

let page="demo"
let slide=0

function doCommand(e) {

  let cmd = String.fromCharCode(e.keyCode).toLowerCase();
  if (cmd==="(") {
    slide++;
    router.push("/multispectral")
  } else {
    slide--;
    router.push("/thermal")
  }
  // do stuff
  console.log(cmd);
  console.log("Page : "+page+" slide : "+slide)

}

</script>

<template>
  <header>
    <img alt="Vue logo" class="logo" src="@/assets/orange-logo.png" />

    <div class="wrapper">
      <nav>
        <RouterLink to="/thermal" class="btn btn-inverse btn-primary menuitem">Thermal</RouterLink>
        <RouterLink to="/multispectral" class="btn btn-inverse btn-primary menuitem">Multi Spectral</RouterLink>
        <RouterLink to="/about" class="btn btn-inverse btn-primary menuitem">About</RouterLink>
      </nav>
    </div>
  </header>

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

.menuitem {
  font-size: 24px;
  margin-right: 2vw;
  margin-left: 2vw;
  margin-top: 2em;
  min-width: 20vw;
}


nav {
  width: 100vw;
  font-size: 24px;
  text-align: center;
  margin: 0;
}

.routerview {
  width: 100vw;
  max-height: 85vh;
  height:85vh;
}

</style>
