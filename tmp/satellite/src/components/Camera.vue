<script setup>
import { reactive, computed,ref } from 'vue'
import axios from 'axios'
const props = defineProps({
  mode: {
    type: String,
    required: true
  },
  title: {
    type: String,
    required: true
  }
})

//var mode="thermal";
var nocache=0;

var imageUrl=  ref("http://192.168.1.69:8080/api/camera/"+props.mode+"?nocache="+nocache) 
//var imageUrl=  ref("@/assets/mlx90641.png")src="@/assets/mlx90641.png"

var fullscreen= ref("/fullscreen/"+props.mode) 

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
  <div class="camera">
    <!--<img :src="imageUrl"><br>-->
    <div class="left"><img src="@/assets/mlx90641.png" class="imgcam center"></div>
    <div class="menu">
      <span class="titre center">{{title}}</span>
      <button @click="updateImage" type="button" class="btn btn-primary menuitem center">Update the Image</button>
      <RouterLink :to="fullscreen" class="btn btn-inverse btn-primary menuitem center">Full Screen</RouterLink>
    </div>
  </div>
</template>

<style scoped>
.titre {
  color: white;
  text-align: center;
  margin-bottom: 30px;
  font-size: 24px;
  font-weight: bold;
}
.column {
  float: left;
  width: 50vw;
}

.camera {
  border: 2px solid white;
  display: flex;
  margin: 5px;
  padding: 10px;
  
}
.left {
  float: left;
  width: 70vw;
}
.imgcam {
    max-width: 100%;
    display: block;
    margin: 10px;
}
.center {
  display: block;
  margin-left: auto;
  margin-right: auto;
}

.menuitem {
  margin-top: 20px;
  width: 15vw;
  text-align: center;
}
.menu {
  float: left;
  display: block;
  width: 30vw;
}
</style>
