<script setup>

import { onMounted } from 'vue'

function drawLine(ctx, begin, end, stroke = 'black', width = 1) {
    if (stroke) {
        ctx.strokeStyle = stroke;
    }

    if (width) {
        ctx.lineWidth = width;
    }

    ctx.beginPath();
    ctx.moveTo(...begin);
    ctx.lineTo(...end);
    ctx.stroke();
}



onMounted(() => {
  console.log(`the component is now mounted.`)
  const canvas = document.querySelector('#canvas');
if (canvas.getContext) {
    const ctx = canvas.getContext('2d');
    drawLine(ctx, [100, 150], [400, 150], 'black', 1);
    for (var i=100;i<=400;i+=20) drawLine(ctx, [i, 50], [i, 250], 'black', 1);
    for (var i=100;i<=400;i+=5) drawLine(ctx, [i, 100], [i, 200], 'black', 1);
}
})


</script>
<template>
<canvas id="canvas" height="400" width="500"></canvas>
</template>

<style scoped>
#canvas {
  background: white;
}
</style>
