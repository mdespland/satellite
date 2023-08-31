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
const { libcamera } = require('libcamera');
const sharp = require('sharp');
const fs = require('fs').promises;
const Gpio = require('pigpio').Gpio;

const PULSE_WIDTH_0 = 500; // Pulse width in microseconds for 0 degrees
const PULSE_WIDTH_180 = 2000; // Pulse width in microseconds for 180 degrees

async function moveServoTo(angle) {
  let motor = new Gpio(18, {mode: Gpio.OsUTPUT});
  let microSeconds = PULSE_WIDTH_0 + (angle / 180) * (PULSE_WIDTH_180 - PULSE_WIDTH_0);
  await motor.servoWrite(Math.round(microSeconds));
  return true;
}

//no filter = 170
// blue = 10
// green = 61
// red = 115

modes= {
  "nofilter" : 170,
  "blue" : 10,
  "green" : 61,
  "red" : 115
}
box={ width: 1920, height: 1080, left: 60, top: 40 }

module.exports = {
  async updateCamera(mode) {
    if (! modes.hasOwnProperty(mode)) mode="nofilter"
    await moveServoTo(modes[mode]);
    await libcamera.jpeg({ config: { output: 'images/'+mode+'.jpg' } });
    return true;
  },
  async getCamera1(mode) {
    const data = await fs.readFile('images/'+mode+'.jpg', "binary")
    return Buffer.from(data, 'binary');
  },
  async getCamera(mode) {
    if (! modes.hasOwnProperty(mode)) mode="nofilter"
    const data = await sharp('images/'+mode+'.jpg').toBuffer()
    return data;
  }  
};