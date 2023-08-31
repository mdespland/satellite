
const { libcamera } = require('libcamera');

libcamera
  .jpeg({ config: { output: 'test.jpg' } })
  .then(result => console.log(result))
  .catch(err => console.log(err));
