<script setup>
import { ref, inject, onMounted, onUnmounted } from 'vue'
const page = inject('page')
const language = inject('language')
page.value="slides"
language.value="fr"
const slide = inject('slide')
const props = defineProps({
  slide: {
    type: Number,
    required: true
  }
})
slide.value=props.slide
console.log("Slides "+props.slide+" "+language.value)
//var imageUrl=  ref(require("@/assets/slides/slide-"+props.slide+".png"))
</script>

<template>
  <div class="main">
    <div v-if="slide == 0" class="image"><img alt="slide" class="icon" src="@/assets/slides/slide-fr-0.png" /></div>
    <div v-if="slide == 1" class="image"><img alt="slide" class="icon" src="@/assets/slides/slide-fr-1.png" /></div>
    <div v-if="slide == 2" class="image"><img alt="slide" class="icon" src="@/assets/slides/slide-fr-2.png" /></div>
    <div v-if="slide == 3" class="image"><img alt="slide" class="icon" src="@/assets/slides/slide-fr-3.png" /></div>
    <div v-if="slide == 6" class="image"><img alt="slide" class="icon" src="@/assets/slides/slide-fr-6.png" /></div>
    <div v-if="slide == 7" class="image"><img alt="slide" class="icon" src="@/assets/slides/slide-fr-7.png" /></div>
    <div v-if="slide == 8" class="image"><img alt="slide" class="icon" src="@/assets/slides/slide-fr-8.png" /></div>
    <div v-if="slide == 9" class="image"><img alt="slide" class="icon" src="@/assets/slides/slide-fr-9.png" /></div>
    <div v-if="slide == 10" class="image"><img alt="slide" class="icon" src="@/assets/slides/slide-fr-10.png" /></div>
    <div v-if="slide == 11" class="image"><img alt="slide" class="icon" src="@/assets/slides/slide-fr-11.png" /></div>
    <div v-if="slide == 12" class="image"><img alt="slide" class="icon" src="@/assets/slides/slide-fr-12.png" /></div>
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