/* const lib = require("./../build/Release/binding.node")
const { process } = lib;

const MODULE_NAME = "explorer.exe";
const SUB_MODULE_NAME = "Explorer.EXE";
const pid = process.findByName(MODULE_NAME);
const info = process.getInfo(pid);

console.log('# pid', pid);
console.log('# info', info);

console.log('# getProcessName', process.getProcessName(pid))

const pmodule = process.getModule(pid, SUB_MODULE_NAME);
console.log('# module', pmodule.base.toString(16).toUpperCase());
console.log('# size', pmodule.size.toString(16).toUpperCase());
console.log('# end', pmodule.end.toString(16).toUpperCase());

const address = process.allocateMemory(pid);
console.log('# Allocated memory at address', address.toString(16).toUpperCase());

const buffer = Buffer.from([1, 2, 3]);
console.log('# Writing memory', buffer, process.writeMemory(pid, address, buffer));

const read = process.readMemory(pid, address, buffer.byteLength);
console.log('# Read memory', read);

const deallocated = process.deallocateMemory(pid, address, buffer.byteLength);
console.log('# Deallocate memory', deallocated);

console.log('# End'); */
import { Crazymem } from '../src/lib';
import { Crazymem as CrazymemClass } from '../src/crazymem';
import 'should';

describe('Crazymem - Integration tests', () => {
  it('should return a Crazymem constructor', () => {
    Crazymem.should.be.a.Function();
  });

  it('should create a Crazymem instance', () => {
    const crazymem = Crazymem('explorer.exe');
    crazymem.should.be.instanceOf(CrazymemClass);
    crazymem.pid.should.be.a.Number();
    crazymem.pid.should.be.above(0);
  });

  it('should return process info and openProcess handler', () => {
    const crazymem = Crazymem('explorer.exe');
    const info = crazymem.GetProcessInfo();
    info.name.should.be.a.String();
    info.pid.should.be.a.Number();
    info.pid.should.be.above(0);
    info.handle.should.be.a.Number();
    info.handle.should.be.above(0);
  });

  it('should check if process is running', () => {
    const crazymem = Crazymem('explorer.exe');
    const info = crazymem.IsProcessRunning();
    info.should.be.eql(true);
  });

  it("should fail as process doesn't exists", () => {
    try {
      Crazymem('gmisland.exe');
    } catch (error) {
      error.message.should.be.eql('NO_PROCESS_FOUND');
    }
  });

  it('should check that multiple instances work as expected', () => {
    const crazymem = Crazymem('explorer.exe');
    crazymem.should.be.instanceOf(CrazymemClass);
    crazymem.pid.should.be.a.Number();
    crazymem.pid.should.be.above(0);

    const subcrazymem = Crazymem('svchost.exe');
    subcrazymem.should.be.instanceOf(CrazymemClass);
    subcrazymem.pid.should.be.a.Number();
    subcrazymem.pid.should.be.above(0);
  });
});