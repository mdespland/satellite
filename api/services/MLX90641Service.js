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
const bindings = require("bindings")("mlx90641");
const Servo = require('../../lib/servo')

const width = 12;
const height = 16;
//const servo=new Servo();

angle = 61;

module.exports = {
    async updateCamera() {
        await servo.moveTo(angle);
        await new Promise(r => setTimeout(r, 2000));
        var pixels = await bindings.capture();
        await servo.stop();
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