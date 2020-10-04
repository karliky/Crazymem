const { process: crazy } = require("../build/Release/binding.node");
import { PID, ProcessName } from './types';

export class Crazymem {
    pid: PID;
    
    constructor(process_name: ProcessName) {
        this.pid = crazy.findByProcessName(process_name);
    }

    GetProcessInfo() {
        return crazy.getInfo(this.pid);
    }

    IsProcessRunning() {
        return crazy.isProcessRunning(this.pid);
    }

    Write(address, buffer) {
        return crazy.writeMemory(this.pid, address, buffer);
    }

    Read(address, buffer) {
        return crazy.readMemory(this.pid, address, buffer);
    }

    PatternScan() {
        const pattern = Buffer.from([0x10, 0x20, 0x0]);
        const mask = 'xx?';
        return crazy.patternScan(this.pid, pattern, mask);
    }

    GetProcessList() {
        return crazy.getProcessList();
    }
}