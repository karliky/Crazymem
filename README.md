<h1>
  <br>
  <img src="resources/logo.png" alt="Crazymem">
  <br>
</h1>

Memory management library for Node.js (Windows/Linux).
Written with [Nan](https://github.com/nodejs/nan) so it works for any Node.js version out of the box.

<p align="center">
  <img src="https://github.com/karliky/crazymem/workflows/CI/badge.svg" />
  <a href="https://github.com/karliky"><img src="https://img.shields.io/badge/Ask%20me-anything-1abc9c.svg" /></a>
  <a href="https://github.com/karliky/Crazymem/issues"><img src="https://img.shields.io/github/issues/karliky/Crazymem.svg" /></a>
</p>


# Features
- 💻 Written in C++ and TypeScript
- 🕹️ Easy to use and well tested
- ⚡ Crazy fast and ready for production
- 🔱 Works with any Node.js version and Electron
- 😎 Synchronous so you don't have to wait

# Usage

Install the library:
```
$ npm install --save crazymem
```

Require:
```
const Crazymem = require("crazymem").Crazymem("WoW.exe");
```

# License
The unlicense. Read 'LICENSE'.

# Methods implemented
```
✅ LM_GetProcessIdEx
✅ LM_GetParentIdEx
✅ LM_OpenProcessEx
✅ LM_GetProcessPathEx
✅ LM_GetProcessNameEx
✅ LM_GetProcessBitsEx
✅ LM_EnumThreadsEx
✅ LM_GetThreadIdEx
✅ LM_EnumModulesEx
✅ LM_GetModuleEx
✅ LM_GetModulePathEx
✅ LM_GetModuleNameEx
✅ LM_LoadModuleEx
✅ LM_UnloadModuleEx
✅ LM_EnumSymbolsEx
✅ LM_GetSymbolEx
✅ LM_EnumPagesEx
✅ LM_GetPageEx
✅ LM_ReadMemoryEx
✅ LM_WriteMemoryEx
✅ LM_SetMemoryEx
✅ LM_ProtMemoryEx
✅ LM_AllocMemoryEx
✅ LM_FreeMemoryEx
✅ LM_DataScanEx
✅ LM_PatternScanEx
✅ LM_SigScanEx
✅ LM_SystemCallEx
✅ LM_FunctionCallEx
✅ LM_DetourCodeEx
✅ LM_MakeTrampolineEx
✅ LM_DestroyTrampolineEx
```

# Thanks to
- Libmem for making the underlying C logic https://github.com/rdbo/libmem

