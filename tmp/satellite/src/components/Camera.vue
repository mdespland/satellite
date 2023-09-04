<script setup>
import { reactive, computed,ref } from 'vue'
import axios from 'axios'
const props = defineProps({
  mode: {
    type: String,
    required: true
  }
})

//var mode="thermal";
var nocache=0;

var imageUrl=  ref("http://192.168.1.69:8080/api/camera/"+props.mode+"?nocache="+nocache) 

function updateImage() {
    axios
      .put("http://192.168.1.69:8080/api/camera/"+props.mode)
      .then(response => {
        nocache++;
        imageUrl.value="http://192.168.1.69:8080/api/camera/"+props.mode+"?nocache="+nocache
        console.log("Change mode to "+mode+" "+imageUrl)
})
    
}

</script>

<template>
  <div class="greetings">
    <center><img :src="imageUrl"><br>
    <button @click="updateImage">Update the Image</button></center>
  </div>
</template>

<style scoped>
.greetings {
  width: 100%;
}
h1 {
  font-weight: 500;
  font-size: 2.6rem;
  position: relative;
  top: -10px;
}
img {

    max-width: 100%;
}
</style>
