const CM = require('../build/Release/addon.node');
const pid = CM.LM_GetProcessIdEx("WoW.exe");
const process = CM.LM_OpenProcessEx(pid);
const pattern = Buffer.from(' (build ');
console.log('process, pattern, 8, process.base, process.end', process, pattern, 8, 0, 0xFFFFFFF)
console.log('# .LM_DataScanEx RESULT ->', CM.LM_DataScanEx(process, pattern, 8, 0, 0xFFFFFFF));
/* const CM = require('../build/Release/addon.node');
const pid = CM.LM_GetProcessIdEx("WoW.exe");
const process = CM.LM_OpenProcessEx(pid);
console.log('# process', process);
console.log('# .LM_GetProcessPathEx RESULT ->', CM.LM_GetProcessPathEx(process));
console.log('# .LM_GetProcessNameEx RESULT ->', CM.LM_GetProcessNameEx(process));
console.log('# .LM_GetProcessBitsEx RESULT ->', CM.LM_GetProcessBitsEx(process));
console.log('# .LM_EnumThreadsEx RESULT ->'   , CM.LM_EnumThreadsEx(process, (tid) => {
    console.log('$$$$ JS LM_EnumThreadsEx', tid);
    return false;
})
);
console.log('# .LM_GetThreadIdEx', CM.LM_GetThreadIdEx(process));
console.log('# .LM_EnumModulesEx RESULT ->'   , CM.LM_EnumModulesEx(process, (module) => {
    console.log('$$$$ JS LM_EnumModulesEx', module);
    return false;
})
);
console.log('# .LM_GetModuleEx RESULT ->', CM.LM_GetModuleEx(process, CM.LM_GetProcessPathEx(process)));
console.log('# .LM_GetModulePathEx RESULT ->', CM.LM_GetModulePathEx(process, CM.LM_GetModuleEx(process, CM.LM_GetProcessPathEx(process))));
console.log('# .LM_GetModuleNameEx RESULT ->', CM.LM_GetModuleNameEx(process, CM.LM_GetModuleEx(process, CM.LM_GetProcessPathEx(process))));
const loadModule = CM.LM_LoadModuleEx("../hello-world-x64.dll");
console.log('# .LM_LoadModuleEx RESULT ->', loadModule);
console.log('# .LM_LoadModuleEx RESULT ->', CM.LM_UnloadModuleEx(process, loadModule));
console.log('# .LM_EnumSymbolsEx RESULT ->', CM.LM_EnumSymbolsEx(process, CM.LM_GetModuleEx(process, CM.LM_GetProcessPathEx(process)), (symbol) => {
    console.log('$$$$ JS LM_EnumSymbolsEx', symbol);
    return false;
}));
console.log('# .LM_EnumPagesEx RESULT ->', CM.LM_EnumPagesEx(process, (page) => {
    console.log('$$$$ JS LM_EnumPagesEx', page);
    return false;
}));
console.log('# .LM_GetPageEx RESULT ->', CM.LM_GetPageEx(process, CM.LM_GetModuleEx(process, CM.LM_GetProcessPathEx(process)).base));
const page = CM.LM_GetPageEx(process, CM.LM_GetModuleEx(process, CM.LM_GetProcessPathEx(process)).base);
console.log('$# page', page.base);
const address = 0xec465ffdfc;
const read = CM.LM_ReadMemoryEx(process, address , 4);
console.log('# .LM_ReadMemoryEx RESULT ->', read, read.readInt32LE(0));
read.writeInt32LE(654321);
console.log('# .LM_WriteMemoryEx RESULT ->', CM.LM_WriteMemoryEx(process, address , read));
const read1 = CM.LM_ReadMemoryEx(process, address , 4);
console.log('# .LM_ReadMemoryEx RESULT ->', read1, read1.readInt32LE(0));
const pattern = Buffer.from([0xf1, 0xfb, 0x09, 0x00]);
console.log('# .About to call data scan');
console.log('# .LM_DataScanEx RESULT ->', CM.LM_DataScanEx(process, pattern, 4, address - 20, 100), "compared to", address);
const mask = "xxxx";
console.log('# .LM_PatternScanEx RESULT ->', CM.LM_PatternScanEx(process, pattern, mask, address - 20, 30), "compared to", address);

 */