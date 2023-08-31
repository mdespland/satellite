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
const Gpio = require('pigpio').Gpio;
//const motor = new Gpio(18, {mode: Gpio.OsUTPUT});

const PULSE_WIDTH_0 = 500; // Pulse width in microseconds for 0 degrees
const PULSE_WIDTH_180 = 2000; // Pulse width in microseconds for 180 degrees

async function moveServoTo(angle) {
  let motor = new Gpio(18, {mode: Gpio.OsUTPUT});
  let microSeconds = PULSE_WIDTH_0 + (angle / 180) * (PULSE_WIDTH_180 - PULSE_WIDTH_0);
  console.log("micoSeconds = "+microSeconds);
  motor.servoWrite(Math.round(microSeconds));
  return true;
}


let increment = 1;

module.exports = {
  async moveToAngle(angle) {
    console.log("Angle="+angle);
    await moveServoTo(angle);
    return true;
  }
};