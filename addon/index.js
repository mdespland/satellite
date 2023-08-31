const bindings = require("bindings")("addon");
const { padImageData, createBitmapFile } = require('./bitmap');


//bindings.doHeavyMath(2, 1).then(console.log);


const width = 12;
const height = 16;




(async () => {
try {
  var pixels=await bindings.doHeavyMath(24, 4);
  console.log(pixels);
  const imageData = padImageData({
    unpaddedImageData: Buffer.from(pixels),
    width,
    height
  });

await createBitmapFile({
  filename: "checkers.bmp",
  imageData,
  width,
  height,
  bitsPerPixel: 8
});
} catch(e) {

}
})();
