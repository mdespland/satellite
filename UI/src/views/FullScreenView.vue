<script setup>
import FullScreen from '../components/FullScreen.vue'
const props = defineProps({
  mode: {
    type: String,
    required: true
  }
})
</script>

<template>
  <main class="fullscreen">
    <FullScreen :mode="mode" class="fullscreen"/>
  </main>
</template>

<style>

.fullscreen {
  max-height: 85vh;
}

div.fullscreen img {
  max-height: 85vh;
}
</style>