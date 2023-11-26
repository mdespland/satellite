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
  },
  colored: {
    type: String,
    required: false, 
    default: "colored"
  },
})


let colored=[]
if (props.colored==="colored") {
  colored = ["colored"]
} else {

}

const colorMode = ref(colored)
var nocache=0;
var baseUrl="";
var imageUrl=  ref(baseUrl+"/api/camera/"+props.mode+"?nocache="+nocache+"&color="+(colorMode.value.length==1)) 

var fullscreen= ref("/fullscreen/"+props.mode+"/"+(colorMode.value.length==1)) 
var checkboxid= ref("colorChecked"+props.mode) 


function updateImage() {
    axios
      .put(baseUrl+"/api/camera/"+props.mode)
      .then(response => {
        nocache++;
        imageUrl.value=baseUrl+"/api/camera/"+props.mode+"?nocache="+nocache+"&color="+(colorMode.value.length==1)
        console.log("Change mode to "+mode+" "+imageUrl)
})
    
}


function changeColorMode() {
  if (colorMode.value.length==1) {
    console.log("Change Color Mode to colored")
  } else {
    console.log("Change Color Mode to greyscale")
  }
  nocache++;
  imageUrl.value=baseUrl+"/api/camera/"+props.mode+"?nocache="+nocache+"&color="+(colorMode.value.length==1)
  fullscreen.value="/fullscreen/"+props.mode+"/"+(colorMode.value.length==1)
}
</script>

<template>
  <div class="camera">
    <div class="left"><img :src="imageUrl"> class="imgcam center"></div>
    <div class="menu">
      <span class="titre center">{{title}}</span>
      <button @click="updateImage" type="button" class="btn btn-primary menuitem center">Update the Image</button>
      <RouterLink :to="fullscreen" class="btn btn-inverse btn-primary menuitem center">Full Screen</RouterLink>
      <div class="form-check form-switch checkbox" v-if="mode !== 'thermal'">
        <input class="form-check-input" type="checkbox" role="switch" value="colored" :id="checkboxid" checked v-on:change="changeColorMode()" v-model="colorMode">
        <label class="form-check-label" :for="checkboxid">Colored</label>
      </div>
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

.checkbox {
  color: white;
  text-align: center;
  margin-top: 20px;
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
  width: 7vw;
  text-align: center;
}
.menu {
  float: left;
  display: block;
  width: 15vw;
}
</style>
