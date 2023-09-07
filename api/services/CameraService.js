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
const Servo = require('../../lib/servo.js')

const servo=new Servo();

//no filter = 170
// blue = 10
// green = 61
// red = 115
//95+90

modes= {
  "nofilter" : 205,
  "blue" : 42,
  "green" : 95,
  "red" : 150
}
box={ width: 1920, height: 1080, left: 60, top: 40 }

module.exports = {
  async updateCamera(mode) {
    if (! modes.hasOwnProperty(mode)) mode="nofilter"
    await servo.moveTo(modes[mode]);
    await libcamera.jpeg({ config: { output: 'images/'+mode+'.jpg' } });
    await servo.stop();
    return true;
  },
  async getCamera1(mode) {
    const data = await fs.readFile('images/'+mode+'.jpg', "binary")
    return Buffer.from(data, 'binary');
  },
  async getCamera(mode, color=false,left=0,top=0,width=0,height=0, angle=0) {
    if (! modes.hasOwnProperty(mode)) mode="nofilter"
    image=await sharp('images/'+mode+'.jpg');
    if (!color) image=await image.greyscale();
    if (width>0 && height>0) image=await image.extract({left:left, top: top, width: width, height: height})
    if (angle>0) image=await image.rotate(angle);
    return await image.toBuffer();
  }  
};
