const Gpio = require('pigpio').Gpio;

const PULSE_WIDTH_0 = 500; // Pulse width in microseconds for 0 degrees
const PULSE_WIDTH_180 = 2000; // Pulse width in microseconds for 180 degrees

class Servo {

    constructor() {
        this.motor= new Gpio(18, {mode: Gpio.OUTPUT});
    }


    async moveTo(angle) {
        //let motor = new Gpio(18, {mode: Gpio.OsUTPUT});
        let microSeconds = PULSE_WIDTH_0 + (angle / 180) * (PULSE_WIDTH_180 - PULSE_WIDTH_0);
        await this.motor.servoWrite(Math.round(microSeconds));
        return true;
      }
      
      async stop() {
        await this.motor.servoWrite(0);
        return true;
      }
}