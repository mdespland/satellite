//
// Copyright 2018-2020 Orange
//
// See the NOTICE file distributed with this work for additional information
// regarding copyright ownership.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
const sharp = require('sharp');
const fs = require('fs').promises;
const Gpio = require('pigpio').Gpio;
const bindings = require("bindings")("mlx90641");
const { padImageData, createBitmapFile } = require('../../lib/bitmap');


const width = 12;
const height = 16;

const PULSE_WIDTH_0 = 500; // Pulse width in microseconds for 0 degrees
const PULSE_WIDTH_180 = 2000; // Pulse width in microseconds for 180 degrees

async function moveServoTo(angle) {
    let motor = new Gpio(18, { mode: Gpio.OsUTPUT });
    let microSeconds = PULSE_WIDTH_0 + (angle / 180) * (PULSE_WIDTH_180 - PULSE_WIDTH_0);
    await motor.servoWrite(Math.round(microSeconds));
    return true;
}

angle = 61;

/*
// Read a raw array of pixels and save it to a png
const input = Uint8Array.from([255, 255, 255, 0, 0, 0]); // or Uint8ClampedArray
const image = sharp(input, {
  // because the input does not contain its dimensions or how many channels it has
  // we need to specify it in the constructor options
  raw: {
    width: 2,
    height: 1,
    channels: 3
  }
});
await image.toFile('my-two-pixels.png');*/


/*
65536
256

65432 = 0 x (65536) + 255 x (256) + 152

*/

module.exports = {
    async updateCamera() {
        await moveServoTo(angle);
        var pixels = await bindings.capture();
        var raw=new Uint8Array(width*height*3);
        for (i=0;i<width*height;i++) {
            raw[i*3+2]=Math.floor(pixels[i]/(65536));
            raw[(i*3)+1]=Math.floor((pixels[i]%65536)/256);
            raw[(i*3)]=pixels[i]%256;
        }
        console.log(raw);
        const image = sharp(raw, {
            // because the input does not contain its dimensions or how many channels it has
            // we need to specify it in the constructor options
            raw: {
              width: width,
              height: height,
              channels: 3
            }
          });
          await image.resize(width*100,height*100).toFile('images/mlx90641.png')
        return true;
    },
    async getCamera() {
        const data = await sharp('images/mlx90641.png').toBuffer()
        return data;
    }
};