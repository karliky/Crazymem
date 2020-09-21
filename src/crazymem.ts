const { process: crazy } = require("./../build/Release/binding.node");
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

    GetProcessList() {
        return crazy.getProcessList();
    }
}