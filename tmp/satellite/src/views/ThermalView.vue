<script setup>
import Camera from '../components/Camera.vue'
</script>

<template>
  <main class="thermal">
    <Camera mode="thermal" class="thermal" title="Thermal"/>
  </main>
</template>

<style>

.thermal {
  max-height: 85vh;
}

div.thermal img {
  max-height: 75vh;
}
</style>