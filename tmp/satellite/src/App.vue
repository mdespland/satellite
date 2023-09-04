<script setup>
import { RouterLink, RouterView } from 'vue-router'
import Satellite from './components/Satellite.vue'
</script>

<template>
  <header>
    <img alt="Vue logo" class="logo" src="@/assets/logo.svg" width="125" height="125" />

    <div class="wrapper">
      <nav>
        <RouterLink to="/">Home</RouterLink>
        <RouterLink to="/thermal">Thermal</RouterLink>
        <RouterLink to="/multispectral">Multi Spectral</RouterLink>
        <RouterLink to="/about">About</RouterLink>
      </nav>
    </div>
  </header>

  <RouterView class="routerview"/>
</template>

<style scoped>
header {
  line-height: 1.5;
  max-height: 100vh;
  /*position: fixed; 
  top: 0;*/
  background:blue;
  width:100%;
}

.logo {
  display: block;
  margin: 0 auto 2rem;
}

nav {
  width: 100%;
  font-size: 12px;
  text-align: center;
  margin-top: 2rem;
}

nav a.router-link-exact-active {
  color: var(--color-text);
}

nav a.router-link-exact-active:hover {
  background-color: transparent;
}

nav a {
  display: inline-block;
  padding: 0 1rem;
  border-left: 1px solid var(--color-border);
}

nav a:first-of-type {
  border: 0;
}
.routerview {
  background:green;
  width: 100%;
}

</style>
