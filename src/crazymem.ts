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
        this.pid = this.LM_GetProcessIdEx(process_name);
        this.process = this.LM_OpenProcessEx(this.pid);
        this.path = this.LM_GetProcessPathEx(this.process);
        const module = this.LM_GetModuleEx(this.process, this.path);
        this.base = module.base;
        this.end = module.end;
        this.size = module.size;
        this.name = this.LM_GetModuleNameEx(this.process, module);
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
    LM_GetProcessPathEx(process: PROCESS): PROCESS_PATH {
        return Native.LM_GetProcessPathEx(process);
    }
    LM_GetProcessNameEx(process: PROCESS): PROCESS_NAME {
        return Native.LM_GetProcessNameEx(process);
    }
    LM_GetProcessBitsEx(process: PROCESS): PROCESS_BITS {
        return Native.LM_GetProcessBitsEx(process);
    }
    LM_EnumThreadsEx(process: PROCESS, cb: ENUM_THREADS_CALLBACK): void {
        Native.LM_EnumThreadsEx(process, cb);
    }
    LM_GetThreadIdEx(process: PROCESS): THREAD_ID {
        return Native.LM_GetThreadIdEx(process);
    }
    LM_EnumModulesEx(process: PROCESS, cb: ENUM_MODULES_CALLBACK): void {
        Native.LM_EnumThreadsEx(process, cb);
    }
    LM_GetModuleEx(process: PROCESS, process_path: PROCESS_PATH): MODULE {
        return Native.LM_GetModuleEx(process, process_path);
    }
    LM_GetModulePathEx(process: PROCESS, module: MODULE): MODULE_PATH {
        return Native.LM_GetModulePathEx(process, module);
    }
    LM_GetModuleNameEx(process: PROCESS, module: MODULE): MODULE_NAME {
        return Native.LM_GetModuleNameEx(process, module);
    }
    LM_LoadModuleEx(process: PROCESS, dll_path: DYNAMIC_LIBRARY_PATH): MODULE {
        return Native.LM_LoadModuleEx(process, dll_path);
    }
    LM_UnloadModuleEx(process: PROCESS, module: MODULE): BSIZE {
        return Native.LM_UnloadModuleEx(process, module);
    }
    LM_EnumSymbolsEx(process: PROCESS, module: MODULE, cb: ENUM_SYMBOLS_CALLBACK): void {
        Native.LM_EnumSymbolsEx(process, module, cb);
    }
    LM_GetSymbolEx(process: PROCESS, module: MODULE, symstr: SYM_STR): PROCESS {
        return Native.LM_GetSymbolEx(process, module, symstr);
    }
    LM_EnumPagesEx(process: PROCESS, cb: ENUM_MEMORY_PAGES_CALLBACK): void {
        Native.LM_EnumPagesEx(process, cb);
    }
    LM_GetPageEx(process: PROCESS, addr: ADDRESS): MEMORY_PAGE {
        return Native.LM_GetPageEx(process, addr);
    }
    LM_ReadMemoryEx(process: PROCESS, addr: ADDRESS, size: BSIZE): Buffer {
        return Native.LM_ReadMemoryEx(process, addr, size);
    }
    LM_WriteMemoryEx(process: PROCESS, addr: ADDRESS, buffer: Buffer): BSIZE {
        return Native.LM_WriteMemoryEx(process, addr, buffer);
    }
    LM_SetMemoryEx(process: PROCESS, dst: ADDRESS, size: BSIZE, flags: BSIZE): BSIZE {
        return Native.LM_SetMemoryEx(process, dst, size, flags);
    }
    LM_ProtMemoryEx(process: PROCESS, dst: ADDRESS, size: BSIZE, prot:void, oldprot: void) {
        return Native.LM_ProtMemoryEx(process, dst, size, prot, oldprot);
    }
    LM_AllocMemoryEx(process: PROCESS, size: BSIZE, protection: BSIZE): ADDRESS {
        return Native.LM_AllocMemoryEx(process, size, protection);
    }
    LM_FreeMemoryEx(process: PROCESS, dst: ADDRESS, size: BSIZE): boolean {
        return Native.LM_FreeMemoryEx(process, dst, size);
    }
    LM_DataScanEx(process: PROCESS, bytes: Buffer, size: BSIZE, addr: ADDRESS, scansize: BSIZE): ADDRESS {
        return Native.LM_DataScanEx(process, bytes, size, addr, scansize);
    }
    LM_PatternScanEx(process: PROCESS, pattern: Buffer, mask: MASK, addr: ADDRESS, scansize: BSIZE): ADDRESS {
        return Native.LM_PatternScanEx(process, pattern, mask, addr, scansize);
    }
    LM_SigScanEx(process: PROCESS, sig: string, addr: ADDRESS, scansize: BSIZE): ADDRESS {
        return Native.LM_SigScanEx(process, sig, addr, scansize);
    }
    LM_SystemCallEx(process: PROCESS, stack_align: BSIZE, nargs: BSIZE,  nrets: BSIZE): boolean {
        return Native.LM_SystemCallEx(process, stack_align, nargs,  nrets);
    }
    LM_FunctionCallEx(process: PROCESS, stack_align: BSIZE, fnaddr: ADDRESS, nargs: BSIZE,  nrets: BSIZE): boolean {
        return Native.LM_FunctionCallEx(process, stack_align, fnaddr, nargs,  nrets);
    }
    LM_DetourCodeEx(process: PROCESS, src: ADDRESS, dst: ADDRESS, detour: ADDRESS): boolean {
        return Native.LM_DetourCodeEx(process, src, dst, detour);
    }
    LM_MakeTrampolineEx(process: PROCESS, src: ADDRESS, size: BSIZE): ADDRESS {
        return Native.LM_MakeTrampolineEx(process, src, size);
    }
    LM_DestroyTrampolineEx(process: PROCESS, addr: ADDRESS): boolean {
        return Native.LM_DestroyTrampolineEx(process, addr);
    }
}