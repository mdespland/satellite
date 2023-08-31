const Gpio = require('pigpio').Gpio;
const motor = new Gpio(18, {mode: Gpio.OsUTPUT});

const PULSE_WIDTH_0 = 500; // Pulse width in microseconds for 0 degrees
const PULSE_WIDTH_180 = 2000; // Pulse width in microseconds for 180 degrees

function moveServoTo(angle) {
  let microSeconds = PULSE_WIDTH_0 + (angle / 180) * (PULSE_WIDTH_180 - PULSE_WIDTH_0);
  motor.servoWrite(Math.round(microSeconds));
}


let angle = 0;
let increment = 1;

setInterval(() => {
  moveServoTo(angle);

  angle += increment;
  if (angle >= 180) {
    increment = -1;
  } else if (angle <= 0) {
    increment = 1;
  }
}, 25);