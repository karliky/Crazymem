<h1>
  <br>
  <img src="resources/logo.png" alt="Crazymem">
  <br>
</h1>

# Crazymem
Memory management library for Node.js (Windows/Linux).
Written with [Nan](https://github.com/nodejs/nan) so it works for any Node.js version out of the box.

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
const lib = require("crazymem")
const { process } = lib;

const MODULE_NAME = "explorer.exe";
const pid = process.findByName(MODULE_NAME);

console.log('# pid', pid);
```

# License
The unlicense. Read 'LICENSE'.

# Methods implemented
```
✅ mem_ex_get_pid
✅ mem_ex_get_process_name
✅ mem_ex_get_process
✅ mem_ex_get_module
mem_ex_is_process_running
✅ mem_ex_read
✅ mem_ex_write
mem_ex_set
mem_ex_protect
✅ mem_ex_allocate
✅ mem_ex_deallocate
mem_ex_scan
mem_ex_pattern_scan
mem_ex_detour
mem_ex_detour_trampoline
mem_ex_detour_restore
mem_ex_load_library
mem_ex_get_symbol
```

# Thanks to
- Libmem for making the underlying C logic https://github.com/rdbo/libmem

