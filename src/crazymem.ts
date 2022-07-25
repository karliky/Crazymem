const Native = require("../build/Release/addon.node");
import { 
    PID, 
    PROCESS_NAME,
    ADDRESS,
    PROCESS_PATH,
    PROCESS,
    PROCESS_BITS,
    ENUM_THREADS_CALLBACK,
    THREAD_ID,
    ENUM_MODULES_CALLBACK,
    MODULE,
    MODULE_PATH,
    MODULE_NAME,
    DYNAMIC_LIBRARY_PATH,
    ENUM_SYMBOLS_CALLBACK,
    SYM_STR,
    MEMORY_PAGE,
    ENUM_MEMORY_PAGES_CALLBACK,
    BSIZE,
    MASK
} from './types';

export class Crazymem {
    pid: PID;
    base: ADDRESS;
    end: ADDRESS;
    size: ADDRESS;
    path: PROCESS_PATH;
    name: PROCESS_NAME;
    process: PROCESS;
    constructor(process_name: PROCESS_NAME) {
        console.log('# process_name', process_name);
        this.pid = this.LM_GetProcessIdEx(process_name);
        console.log('this.pid', this.pid);
        if (this.pid === 0xFFFFFFFF) throw new Error('CANNOT_GET_PROCESS')
        console.log('# pid', this.pid);
        this.process = this.LM_OpenProcessEx(this.pid);
        console.log('# process', this.process);
        this.path = this.LM_GetProcessPathEx();
        const module = this.LM_GetModuleEx(this.path);
        this.base = module.base;
        this.end = module.end;
        this.size = module.size;
        this.name = this.LM_GetModuleNameEx(module);
    }

    LM_GetProcessIdEx(procstr: PROCESS_NAME): PID {
        return Native.LM_GetProcessIdEx(procstr);
    }
    LM_GetParentIdEx(pid: PID) {
        return Native.LM_GetParentIdEx(pid);
    }
    LM_OpenProcessEx(pid: PID): PROCESS {
        return Native.LM_OpenProcessEx(pid);
    }
    LM_GetProcessPathEx(): PROCESS_PATH {
        return Native.LM_GetProcessPathEx(this.process);
    }
    LM_GetProcessNameEx(): PROCESS_NAME {
        return Native.LM_GetProcessNameEx(this.process);
    }
    LM_GetProcessBitsEx(): PROCESS_BITS {
        return Native.LM_GetProcessBitsEx(this.process);
    }
    LM_EnumThreadsEx(cb: ENUM_THREADS_CALLBACK): void {
        Native.LM_EnumThreadsEx(this.process, cb);
    }
    LM_GetThreadIdEx(): THREAD_ID {
        return Native.LM_GetThreadIdEx(this.process);
    }
    LM_EnumModulesEx(cb: ENUM_MODULES_CALLBACK): void {
        Native.LM_EnumThreadsEx(this.process, cb);
    }
    LM_GetModuleEx(process_path: PROCESS_PATH): MODULE {
        return Native.LM_GetModuleEx(this.process, process_path);
    }
    LM_GetModulePathEx(module: MODULE): MODULE_PATH {
        return Native.LM_GetModulePathEx(this.process, module);
    }
    LM_GetModuleNameEx(module: MODULE): MODULE_NAME {
        return Native.LM_GetModuleNameEx(this.process, module);
    }
    LM_LoadModuleEx(dll_path: DYNAMIC_LIBRARY_PATH): MODULE {
        return Native.LM_LoadModuleEx(this.process, dll_path);
    }
    LM_UnloadModuleEx(module: MODULE): BSIZE {
        return Native.LM_UnloadModuleEx(this.process, module);
    }
    LM_EnumSymbolsEx(module: MODULE, cb: ENUM_SYMBOLS_CALLBACK): void {
        Native.LM_EnumSymbolsEx(this.process, module, cb);
    }
    LM_GetSymbolEx(module: MODULE, symstr: SYM_STR): PROCESS {
        return Native.LM_GetSymbolEx(this.process, module, symstr);
    }
    LM_EnumPagesEx(cb: ENUM_MEMORY_PAGES_CALLBACK): void {
        Native.LM_EnumPagesEx(this.process, cb);
    }
    LM_GetPageEx(addr: ADDRESS): MEMORY_PAGE {
        return Native.LM_GetPageEx(this.process, addr);
    }
    LM_ReadMemoryEx(addr: ADDRESS, size: BSIZE): Buffer {
        return Native.LM_ReadMemoryEx(this.process, addr, size);
    }
    LM_WriteMemoryEx(addr: ADDRESS, buffer: Buffer): BSIZE {
        return Native.LM_WriteMemoryEx(this.process, addr, buffer);
    }
    LM_SetMemoryEx(dst: ADDRESS, size: BSIZE, flags: BSIZE): BSIZE {
        return Native.LM_SetMemoryEx(this.process, dst, size, flags);
    }
    LM_ProtMemoryEx(dst: ADDRESS, size: BSIZE, prot:void, oldprot: void) {
        return Native.LM_ProtMemoryEx(this.process, dst, size, prot, oldprot);
    }
    LM_AllocMemoryEx(size: BSIZE, protection: BSIZE): ADDRESS {
        return Native.LM_AllocMemoryEx(this.process, size, protection);
    }
    LM_FreeMemoryEx(dst: ADDRESS, size: BSIZE): boolean {
        return Native.LM_FreeMemoryEx(this.process, dst, size);
    }
    LM_DataScanEx(bytes: Buffer, size: BSIZE, addr: ADDRESS, scansize: BSIZE): ADDRESS {
        console.log('this.process, bytes, size, addr, scansize', this.process, bytes, size, addr, scansize);
        return Native.LM_DataScanEx(this.process, bytes, size, addr, scansize);
    }
    LM_PatternScanEx(pattern: Buffer, mask: MASK, addr: ADDRESS, scansize: BSIZE): ADDRESS {
        return Native.LM_PatternScanEx(this.process, pattern, mask, addr, scansize);
    }
    LM_SigScanEx(sig: string, addr: ADDRESS, scansize: BSIZE): ADDRESS {
        return Native.LM_SigScanEx(this.process, sig, addr, scansize);
    }
    LM_SystemCallEx(stack_align: BSIZE, nargs: BSIZE,  nrets: BSIZE): boolean {
        return Native.LM_SystemCallEx(this.process, stack_align, nargs,  nrets);
    }
    LM_FunctionCallEx(stack_align: BSIZE, fnaddr: ADDRESS, nargs: BSIZE,  nrets: BSIZE): boolean {
        return Native.LM_FunctionCallEx(this.process, stack_align, fnaddr, nargs,  nrets);
    }
    LM_DetourCodeEx(src: ADDRESS, dst: ADDRESS, detour: ADDRESS): boolean {
        return Native.LM_DetourCodeEx(this.process, src, dst, detour);
    }
    LM_MakeTrampolineEx(src: ADDRESS, size: BSIZE): ADDRESS {
        return Native.LM_MakeTrampolineEx(this.process, src, size);
    }
    LM_DestroyTrampolineEx(addr: ADDRESS): boolean {
        return Native.LM_DestroyTrampolineEx(this.process, addr);
    }
}

import Utils from './utils/robot-js';

export const utils = Utils;