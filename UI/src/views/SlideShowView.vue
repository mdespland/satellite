<script setup>
import { ref, inject, onMounted, onUnmounted } from 'vue'
const page = inject('page')
page.value="slideshow"

let slide=ref(1)
let startId;

onMounted(() => {
  startId = setInterval(nextSlide, 5000);
});

onUnmounted(() => {
  clearInterval(startId);
});


function nextSlide() {
  slide.value++;
  if (slide.value>10) slide.value=1;
  console.log("Changing to slide "+slide.value)
}
</script>

<template>
  <div class="main">
    <div v-if="slide == 1" class="image"><img alt="slide" class="icon" src="@/assets/slides/slide1.png" /></div>
    <div v-if="slide == 2" class="image"><img alt="slide" class="icon" src="@/assets/slides/slide2.png" /></div>
    <div v-if="slide == 5" class="image"><img alt="slide" class="icon" src="@/assets/slides/slide5.png" /></div>
    <div v-if="slide == 6" class="image"><img alt="slide" class="icon" src="@/assets/slides/slide6.png" /></div>
    <div v-if="slide == 7" class="image"><img alt="slide" class="icon" src="@/assets/slides/slide7.png" /></div>
    <div v-if="slide == 8" class="image"><img alt="slide" class="icon" src="@/assets/slides/slide8.png" /></div>
    <div v-if="slide == 9" class="image"><img alt="slide" class="icon" src="@/assets/slides/slide9.png" /></div>
    <div v-if="slide == 10" class="image"><img alt="slide" class="icon" src="@/assets/slides/slide10.png" /></div>
  </div>
</template>

<style  scoped>
.main {
  width: 100vw;
  display: flex;
  min-height: 100vh;
}
.image {
  display: flex;
  align-items: center;
  justify-content: center;
  height:  100vh;
  width:  100vw;    
}

.icon {
    align: center;
  height:  80vh;
  width:  80vw;
}
</style>