// test.js
const addon = require('../build/Release/addon');

console.log('This should be eight:', addon.add(3, 5)); 

addon.run((msg) => {
    console.log(msg);
  // Prints: 'hello world'
  }); 


addon.getThermoImage((image) => {
    console.log("Thermo Image");
    console.log(JSON.toString(image));
  // Prints: 'hello world'
}); 

  async function help() {
    console.log("test");

  }
  (async () => {
  try {
    await help();
  } catch(e) {

  }
})();
  