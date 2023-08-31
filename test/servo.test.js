const assert = require('assert');
const chai = require('chai');
const chaiAsPromised = require('chai-as-promised');

chai.use(chaiAsPromised);

const expect = chai.expect;

const Gpio = require('pigpio').Gpio;
const motor = new Gpio(18, {mode: Gpio.OsUTPUT});

const PULSE_WIDTH_0 = 1000; // Pulse width in microseconds for 0 degrees
const PULSE_WIDTH_180 = 2000; // Pulse width in microseconds for 180 degrees

async function moveServoTo(angle) {
  let microSeconds = PULSE_WIDTH_0 + (angle / 180) * (PULSE_WIDTH_180 - PULSE_WIDTH_0);
  await motor.servoWrite(Math.round(microSeconds));
  await delay(500)
}
function delay(time) {
    return new Promise(resolve => setTimeout(resolve, time));
  } 

describe('Servo Motor Control', () => {
    it('Move Forward', async () => {
        var status="OK"
        pulseWidth=2500
        //console.log(motor.getServoPulseWidth())
        
        await moveServoTo(10)
        expect(status).to.eql("OK")
    })
});

