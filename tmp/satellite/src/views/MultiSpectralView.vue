<script setup>
import Camera from '../components/Camera.vue'
</script>

<template>
  <div class="main">
    <div class="row">
      <div class="column">
         <Camera mode="nofilter"/>
         <Camera mode="green"/>
         </div>
      <div class="column">
        <Camera mode="red"/>
        <Camera mode="blue"/>
      </div>
    </div>
  </div>
</template>

<style scoped>
.main {
  width: 100%;
  display: flex;
  background: red;
  min-height: 100vh;
}
.column {
  float: left;
  width: 50%;
  background: blue;
}
.row {
  width: 100%;
}

/* Clear floats after the columns */
.row:after {
  content: "";
  display: table;
  clear: both;
}
</style>