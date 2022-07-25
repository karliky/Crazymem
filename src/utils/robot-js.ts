// This is a set of bindings for Robot-js API

function initialize(cb) {
    var Crazymem = require('../../').Crazymem("WoW.exe");
    
    const process = {};
    const memory = {
        readMultiLevelPtr: (offsets: any, noModule: any) => {},
        resolvePtrBySetOfInstruction: (patternBase: any, build: any, ptrFix: any) => {},
        findPattern: (pattern: any) => {},
        findStrPattern: (str: string) => {},
        find: (bytes, size, addr = Crazymem.base, scansize = Crazymem.end) => [Crazymem.LM_DataScanEx(bytes, size, addr, scansize)],
        readPtr: (address, size = 4) => Crazymem.LM_ReadMemoryEx(address, size).readInt32LE(0),
        writeData: (address, buffer) => Crazymem.LM_WriteMemoryEx(address, buffer),
        readData: (address, outputBuffer, size = 4) => {
            const readData = Crazymem.LM_ReadMemoryEx(address, size);
            for (let index = 0; index < size; index++) {
                outputBuffer.writeUInt8(readData[index], index);
            }
        },
    };
    const module = Crazymem.base;
    const window = {};

    function readMultiLevelPtr(offsets, noModule) {
        let mod = module;
        if (noModule) mod = 0;
        let address = mod + Number(offsets[0]);
        for (let i = 1; i < offsets.length; i += 1) {
            address = memory.readPtr(address);
            address += offsets[i];
        }
        return address;
    }

    const ptrContainer = Buffer.alloc(0x4);

    const reverseArrayOfBytesAsNumber = buffPtr => parseInt(buffPtr.toString('hex').match(/[a-fA-F0-9]{2}/g).reverse().join(''), 16);

    const findPattern = pattern => memory.find(pattern, pattern.length, Crazymem.base, Crazymem.end);

    function resolvePtrBySetOfInstruction(patternBase, build, ptrFix) {
        ptrFix = ptrFix || 0;
        const hexPattern = patternBase.pattern;
        let patternPtr = findPattern(hexPattern);
        if (patternPtr.length === 0) return;
        patternPtr = patternPtr.shift();
        patternPtr += patternBase.patternFix[build];
        memory.readData(patternPtr, ptrContainer, ptrContainer.byteLength);
        const ptr = reverseArrayOfBytesAsNumber(ptrContainer);
        if (Array.isArray(ptrFix)) {
            const path = [(ptr - module)].concat(ptrFix);
            return readMultiLevelPtr(path, false);
        }
        return memory.readPtr((ptr + ptrFix));
    }


    const findStrPattern = (str) => {
        const searchPattern = Buffer.from(str);
        const scan = Crazymem.LM_DataScanEx(searchPattern, searchPattern.byteLength / 2, Crazymem.base, Crazymem.end);
        return [scan];
    };

    memory.readMultiLevelPtr = readMultiLevelPtr;
    memory.resolvePtrBySetOfInstruction = resolvePtrBySetOfInstruction;
    memory.findPattern = findPattern;
    memory.findStrPattern = findStrPattern;

    cb(null, process, module, memory, window);
}

export default initialize;