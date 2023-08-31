const assert = require('assert');
const chai = require('chai');
const chaiAsPromised = require('chai-as-promised');
//simport { libcamera } from 'libcamera';
const { libcamera } = require('libcamera');
chai.use(chaiAsPromised);

const expect = chai.expect;


describe('Libcamera', () => {
    it('Save JPEG', async () => {
        
        await libcamera.jpeg({ config: { output: 'test.jpg' } });
        expect("OK").to.eql("OK")
    }).timeout(10000);
}).timeout(10000);

