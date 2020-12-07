const { process: crazy } = require("../build/Release/binding.node");
import { PID, PROCESS_NAME, ADDRESS, PROCESS_PATH, PTR_LENGTH } from './types';

export class Crazymem {
    pid: PID;
    base: ADDRESS;
    end: ADDRESS;
    size: ADDRESS;
    path: PROCESS_PATH;
    name: PROCESS_NAME;
    constructor(process_name: PROCESS_NAME) {
        this.pid = crazy.findByProcessName(process_name);
        const { 
            base,
            end,
            name,
            path,
            size
        } = crazy.getModule(this.pid, process_name);
        this.base = base;
        this.end = end;
        this.name = name;
        this.path = path;
        this.size = size;
    }

    GetProcessInfo() {
        return crazy.getInfo(this.pid);
    }

    IsProcessRunning() {
        return crazy.isProcessRunning(this.pid);
    }

    GetModule(moduleName: string) {
        return crazy.getModule(this.pid, moduleName);
    }

    Write(address, buffer) {
        return crazy.writeMemory(this.pid, address, buffer);
    }

    Read(address, size) {
        return crazy.readMemory(this.pid, address, size);
    }

    getOffset(addresses) {
        let address = addresses[0];
        for (let i = 1; i < addresses.length; i++) {
            address = this.Read(address, PTR_LENGTH).readUInt32LE(0);
            address += addresses[i];
        }
        return address;
    }

    PatternScan(pattern: Buffer, mask: string) {
        return crazy.patternScan(this.pid, pattern, mask);
    }

    GetProcessList() {
        return crazy.getProcessList();
    }

    CheckKeyIsPressed(key_code: number) {
        return crazy.getKeyState(key_code);
    }
}