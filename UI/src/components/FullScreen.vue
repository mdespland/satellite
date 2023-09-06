<script setup>
import { reactive, computed,ref } from 'vue'
import axios from 'axios'
const props = defineProps({
  mode: {
    type: String,
    required: true
  },
  colored: {
    type: Boolean,
    required: true
  }
})
var baseUrl="";
var nocache=0;

var imageUrl=  ref(baseUrl+"/api/camera/"+props.mode+"?nocache="+nocache+"&color="+props.colored) 
//var imageUrl=  ref("@/assets/mlx90641.png")src="@/assets/mlx90641.png"

</script>

<template>
  <div class="camera">
    <img :src="imageUrl" class="center">
  </div>
</template>

<style scoped>

.camera {
  display: block;
  
}
img {
    max-width: 100%;
    display: block;
}
.center {
  display: block;
  margin-left: auto;
  margin-right: auto
}
</style>
