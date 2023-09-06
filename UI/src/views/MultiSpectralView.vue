<script setup>
import Camera from '../components/Camera.vue'
</script>

<template>
  <div class="main">
    <div class="row">
      <div class="column">
         <Camera mode="nofilter" class="multispetral" title="No Filter"/>
         <Camera mode="green" class="multispetral" title="Green"/>
         </div>
      <div class="column">
        <Camera mode="red" class="multispetral" title="Red"/>
        <Camera mode="blue" class="multispetral" title="Blue"/>
      </div>
    </div>
  </div>
</template>

<style  scoped>
.main {
  width: 100vw;
  display: flex;
  min-height: 40vh;
}
.column {
  float: left;
  width: 50vw;
  height: 85vh;
}
.row {
  width: 100vw;
  height: 45vh;
}

/* Clear floats after the columns */
.row:after {
  content: "";
  display: table;
  clear: both;
}

</style>

<style>
div.multispetral img {
  max-height: 38vh;
}
div.multispetral camera {
  height: 35vh;
}
</style>