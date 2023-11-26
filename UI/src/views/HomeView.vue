<script setup>
import { RouterLink} from 'vue-router'
import { inject } from 'vue'
const page = inject('page')
page.value="home"
</script>

<template>
  <main class="fullscreen">
    <div class="row">
      <div class="column">
        <RouterLink to="/multispectral"><center><img alt="Vue logo" class="icon" src="@/assets/multispectral.png" /></center></RouterLink>
        <RouterLink to="/thermal"><center><img alt="Vue logo" class="icon" src="@/assets/mlx90641.png" /></center></RouterLink>
      </div>
      <div class="column">
        <RouterLink to="/slides/fr/0"><center><img alt="Vue logo" class="icon" src="@/assets/iconslides.png" /></center></RouterLink>
        <RouterLink to="/slides/en/0"><center><img alt="Vue logo" class="icon" src="@/assets/iconslideshow.png" /></center></RouterLink>
      </div>
    </div>
  </main>
</template>


<style>
.table {
  border:0;
  /*align-self: center;*/
}
.fullscreen {
  /*justify-content: center;*/
  width: 100vw;
  height: 100vh;
  max-height: 100vh;
  display: flex;
  align-items: center;
  justify-content: center;
}

/*
.main {
  width: 100vw;
  display: flex;
  min-height: 40vh;
}*/
.column {
  float: left;
  width: 50vw;
  height: 100vh;
  align-items: center;
  justify-content: center;
}
.row {
  width: 100vw;
  height: 100vh;
}

/* Clear floats after the columns */
.row:after {
  content: "";
  display: table;
  clear: both;
}
.icon {
  display: block;
  margin: 20px;
  height:  30vh;
}
</style>