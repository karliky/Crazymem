#include <nan.h>
#include <string.h>
#include "libmem/libmem.h"

#if defined(MEM_UCS)    //Unicode character set
#define print(...) wprintf(MEM_STR("\n") __VA_ARGS__)
#elif defined(MEM_MBCS) //Multibyte character set
#define print(...) printf(MEM_STR("\n") __VA_ARGS__)
#endif

#define PROTECTION PAGE_EXECUTE_READWRITE

#define tprint(...) print("    "  __VA_ARGS__)

#define PROCESS_NAME MEM_STR("explorer.exe")

void FindByProcessName(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  v8::Local<v8::Context> context = info.GetIsolate()->GetCurrentContext();
  
  if (info.Length() == 0) {
    Nan::ThrowTypeError("MISSING_PROCESS_NAME");
    return;
  }
  
  std::string process_name = *Nan::Utf8String(info[0]);
	mem_pid_t pid = mem_ex_get_pid(mem_string_new(process_name.c_str()));
  auto return_value = (double) pid;

  if (return_value == 0xFFFFFFFF) {
    Nan::ThrowTypeError("NO_PROCESS_FOUND");
    return;
  }

  info.GetReturnValue().Set(Nan::New(return_value));
}

void GetProcessName(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  v8::Local<v8::Context> context = info.GetIsolate()->GetCurrentContext();
  
  if (info.Length() != 1) {
    Nan::ThrowTypeError("MISSING_PID");
    return;
  }
  
  auto pid_ex = (DWORD) info[0]->NumberValue(context).FromJust();
	mem_string_t process_name = mem_ex_get_process_name(pid_ex);

  info.GetReturnValue().Set(Nan::New(mem_string_c_str(&process_name)).ToLocalChecked());
}

void GetProcessInfo(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  v8::Local<v8::Context> context = info.GetIsolate()->GetCurrentContext();
  
  if (info.Length() == 0) {
    Nan::ThrowTypeError("MISSING_PROCESS_PID");
    return;
  }
  
  auto pid_ex = info[0]->NumberValue(context).FromJust();
  mem_process_t process_ex = mem_ex_get_process((DWORD) pid_ex);
  v8::Local<v8::Object> obj = Nan::New<v8::Object>();

  obj->Set(context, Nan::New("name").ToLocalChecked(), Nan::New(mem_string_c_str(&process_ex.name)).ToLocalChecked());
  obj->Set(context, Nan::New("pid").ToLocalChecked(), Nan::New((double) process_ex.pid));
  obj->Set(context, Nan::New("handle").ToLocalChecked(), Nan::New((double) reinterpret_cast<int64_t>(process_ex.handle)));

  info.GetReturnValue().Set(obj);
}

void GetProcessModule(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  v8::Local<v8::Context> context = info.GetIsolate()->GetCurrentContext();
  
  if (info.Length() != 2) {
    Nan::ThrowTypeError("MISSING_FUNCTION_ARGUMENTS (pid, module_name)");
    return;
  }
  
  DWORD pid_ex = info[0]->NumberValue(context).FromJust();
  std::string process_name = *Nan::Utf8String(info[1]);

  mem_process_t process_ex = mem_ex_get_process((DWORD) pid_ex);
	mem_module_t process_mod_ex = mem_ex_get_module(process_ex, mem_string_new(process_name.c_str()));
  v8::Local<v8::Object> obj = Nan::New<v8::Object>();

  obj->Set(context, Nan::New("name").ToLocalChecked(), Nan::New(mem_string_c_str(&process_mod_ex.name)).ToLocalChecked());
  obj->Set(context, Nan::New("path").ToLocalChecked(), Nan::New(mem_string_c_str(&process_mod_ex.path)).ToLocalChecked());
  obj->Set(context, Nan::New("base").ToLocalChecked(), Nan::New((double) reinterpret_cast<intptr_t>(process_mod_ex.base)));
  obj->Set(context, Nan::New("size").ToLocalChecked(), Nan::New((double) process_mod_ex.size));
  obj->Set(context, Nan::New("end").ToLocalChecked(), Nan::New((double) reinterpret_cast<intptr_t>(process_mod_ex.end)));

  info.GetReturnValue().Set(obj);
}

/**
 * TODO: Support linux mode
 * TODO: Add test
*/
void LoadLibrary(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  v8::Local<v8::Context> context = info.GetIsolate()->GetCurrentContext();
  
  if (info.Length() != 2) {
    Nan::ThrowTypeError("INVALID_PARAMETERS");
    return;
  }

  DWORD pid_ex = info[0]->NumberValue(context).FromJust();
  mem_process_t process_ex = mem_ex_get_process((DWORD) pid_ex);
  std::string lib_path = *Nan::Utf8String(info[1]);
  mem_lib_t lib = mem_lib_init();
  lib.path = mem_string_new(lib_path.c_str());
  mem_int_t load_library = mem_ex_load_library(process_ex, lib);
  info.GetReturnValue().Set(Nan::New((double) load_library));
}

void GetProcessList(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  v8::Local<v8::Context> context = info.GetIsolate()->GetCurrentContext();

  v8::Local<v8::Array> process_list = Nan::New<v8::Array>();

  mem_process_list_t proc_list = mem_ex_get_process_list();
  for(mem_size_t i = 0; i < MEM_THISCALL(proc_list, length); i++)
  {
      mem_process_t process = MEM_THISCALL(proc_list, at, i);
      mem_module_list_t  mod_list  = mem_ex_get_module_list(process);

      v8::Local<v8::Array> modules = Nan::New<v8::Array>();
      v8::Local<v8::Object> obj = Nan::New<v8::Object>();
      for(mem_size_t j = 0; j < MEM_THISCALL(mod_list, length); j++)
      {
          mem_module_t mod = MEM_THISCALL(mod_list, at, j);
          modules->Set(context, j, Nan::New(mem_string_c_str(&mod.name)).ToLocalChecked());
      }

      obj->Set(context, Nan::New("name").ToLocalChecked(), Nan::New(mem_string_c_str(&process.name)).ToLocalChecked());
      obj->Set(context, Nan::New("modules").ToLocalChecked(), modules);
      process_list->Set(context, i, obj);
  }

  info.GetReturnValue().Set(process_list);
}

void AllocateMemory(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  v8::Local<v8::Context> context = info.GetIsolate()->GetCurrentContext();
  
  if (info.Length() != 1) {
    Nan::ThrowTypeError("INVALID_PARAMETERS");
    return;
  }

  DWORD pid_ex = info[0]->NumberValue(context).FromJust();
  mem_process_t process_ex = mem_ex_get_process((DWORD) pid_ex);
	mem_voidptr_t alloc_ex = mem_ex_allocate(process_ex, sizeof(int), PROTECTION);

  info.GetReturnValue().Set((double) reinterpret_cast<intptr_t>(alloc_ex));
}

void DeallocateMemory(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  v8::Local<v8::Context> context = info.GetIsolate()->GetCurrentContext();
  
  if (info.Length() != 3) {
    Nan::ThrowTypeError("MISSING_FUNCTION_ARGUMENTS (pid, address, buffer)");
    return;
  }

  DWORD pid_ex = info[0]->NumberValue(context).FromJust();
  mem_process_t process_ex = mem_ex_get_process((DWORD) pid_ex);
  auto address = (uintptr_t) info[1]->NumberValue(context).FromJust();
	auto byteLength = (int64_t) info[2]->NumberValue(context).FromJust();
	mem_int_t deallocated = mem_ex_deallocate(process_ex, (void *) address, byteLength);

  info.GetReturnValue().Set((bool) deallocated);
}

void IsProcessRunning(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  v8::Local<v8::Context> context = info.GetIsolate()->GetCurrentContext();
  
  if (info.Length() != 1) {
    Nan::ThrowTypeError("MISSING_FUNCTION_ARGUMENTS (pid)");
    return;
  }

  DWORD pid_ex = info[0]->NumberValue(context).FromJust();
  mem_process_t process_ex = mem_ex_get_process((DWORD) pid_ex);
  auto isRunning = (bool) mem_ex_is_process_running(process_ex);
  info.GetReturnValue().Set(isRunning);
}

void WriteMemory(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  v8::Local<v8::Context> context = info.GetIsolate()->GetCurrentContext();
  
  if (info.Length() != 3) {
    Nan::ThrowTypeError("MISSING_FUNCTION_ARGUMENTS (pid, address, buffer)");
    return;
  }

  auto pid_ex = (DWORD64) info[0]->NumberValue(context).FromJust();
  auto process_ex = (mem_process_t) mem_ex_get_process(pid_ex);
  auto address = (uintptr_t) info[1]->NumberValue(context).FromJust();

  auto buffer = Nan::To<v8::Object>(info[2]).ToLocalChecked();
	auto data = node::Buffer::Data(buffer);
	auto size = node::Buffer::Length(buffer);
	mem_ex_write(process_ex, (void *) address, data, size);

  info.GetReturnValue().Set(true);
}

void ReadMemory(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  v8::Local<v8::Context> context = info.GetIsolate()->GetCurrentContext();
  
  if (info.Length() != 3) {
    Nan::ThrowTypeError("MISSING_FUNCTION_ARGUMENTS (pid, address, byteLength)");
    return;
  }

	auto pid_ex = (DWORD64) info[0]->NumberValue(context).FromJust();
	auto address = (uintptr_t) info[1]->NumberValue(context).FromJust();
	auto byteLength = (int64_t) info[2]->NumberValue(context).FromJust();
  auto process_ex = (mem_process_t) mem_ex_get_process(pid_ex);
  char* output_buffer = new char[byteLength];
	mem_ex_read(process_ex, (void *) address, output_buffer, byteLength);
  Nan::MaybeLocal<v8::Object> output = Nan::NewBuffer(output_buffer, byteLength);
  delete[] output_buffer;
  info.GetReturnValue().Set(output.ToLocalChecked());
}

void PatternScan(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  v8::Local<v8::Context> context = info.GetIsolate()->GetCurrentContext();
  
  if (info.Length() != 3) {
    Nan::ThrowTypeError("MISSING_FUNCTION_ARGUMENTS (pid, pattern, mask)");
    return;
  }

	auto pid_ex = (DWORD64) info[0]->NumberValue(context).FromJust();
  auto process_ex = (mem_process_t) mem_ex_get_process(pid_ex);
  auto buffer = Nan::To<v8::Object>(info[1]).ToLocalChecked();
  std::string mask_definition = *Nan::Utf8String(info[1]);

	// auto pattern = node::Buffer::Data(buffer);
	// mem_string_t mask = mem_string_new(mask_definition.c_str());
	// mem_voidptr_t scan = mem_ex_pattern_scan(process_ex, pattern, mask, (mem_voidptr_t)((mem_uintptr_t) 0), (mem_voidptr_t)((mem_uintptr_t) 0x00007fffffffffff));

	//-- Pattern Scanning
	mem_int8_t pattern[] = { (mem_int8_t)0x10, (mem_int8_t)0x20, (mem_int8_t)0x0, (mem_int8_t)0x30, (mem_int8_t)0x40, (mem_int8_t)0x50,(mem_int8_t)0x60, (mem_int8_t)0x70, (mem_int8_t)0x80, (mem_int8_t)0x90, (mem_int8_t)0xA0, (mem_int8_t)0x00, (mem_int8_t)0xB0 };
	mem_string_t mask = mem_string_new(MEM_STR("xx?xxxxxxxx?x"));
	mem_voidptr_t scan = mem_ex_pattern_scan(process_ex, pattern, mask, (mem_voidptr_t)((mem_uintptr_t) 0), (mem_voidptr_t)((mem_uintptr_t) 0x00007fffffffffff));
	tprint("Pattern Scan:       %p", scan);
	tprint(" (expected result): %p", (mem_voidptr_t)pattern);

  info.GetReturnValue().Set((double) reinterpret_cast<int64_t>(scan));
}

void Init(v8::Local<v8::Object> exports) {
  v8::Local<v8::Context> context = exports->CreationContext();

  v8::Local<v8::Object> obj = Nan::New<v8::Object>();
  obj->Set(context,
           Nan::New("findByProcessName").ToLocalChecked(),
            Nan::New<v8::FunctionTemplate>(FindByProcessName)
                  ->GetFunction(context)
                  .ToLocalChecked());
  obj->Set(context,
          Nan::New("getInfo").ToLocalChecked(),
          Nan::New<v8::FunctionTemplate>(GetProcessInfo)
                ->GetFunction(context)
                .ToLocalChecked());
  obj->Set(context,
          Nan::New("getModule").ToLocalChecked(),
          Nan::New<v8::FunctionTemplate>(GetProcessModule)
                ->GetFunction(context)
                .ToLocalChecked());
  obj->Set(context,
          Nan::New("allocateMemory").ToLocalChecked(),
          Nan::New<v8::FunctionTemplate>(AllocateMemory)
                ->GetFunction(context)
                .ToLocalChecked());
  obj->Set(context,
          Nan::New("deallocateMemory").ToLocalChecked(),
          Nan::New<v8::FunctionTemplate>(DeallocateMemory)
                ->GetFunction(context)
                .ToLocalChecked());
  obj->Set(context,
          Nan::New("writeMemory").ToLocalChecked(),
          Nan::New<v8::FunctionTemplate>(WriteMemory)
                ->GetFunction(context)
                .ToLocalChecked());
  obj->Set(context,
          Nan::New("readMemory").ToLocalChecked(),
          Nan::New<v8::FunctionTemplate>(ReadMemory)
                ->GetFunction(context)
                .ToLocalChecked());
  obj->Set(context,
          Nan::New("getProcessName").ToLocalChecked(),
          Nan::New<v8::FunctionTemplate>(GetProcessName)
                ->GetFunction(context)
                .ToLocalChecked());
  obj->Set(context,
          Nan::New("isProcessRunning").ToLocalChecked(),
          Nan::New<v8::FunctionTemplate>(IsProcessRunning)
                ->GetFunction(context)
                .ToLocalChecked());
  obj->Set(context,
        Nan::New("loadLibrary").ToLocalChecked(),
        Nan::New<v8::FunctionTemplate>(LoadLibrary)
              ->GetFunction(context)
              .ToLocalChecked());
  obj->Set(context,
        Nan::New("getProcessList").ToLocalChecked(),
        Nan::New<v8::FunctionTemplate>(GetProcessList)
              ->GetFunction(context)
              .ToLocalChecked());
  obj->Set(context,
        Nan::New("patternScan").ToLocalChecked(),
        Nan::New<v8::FunctionTemplate>(PatternScan)
              ->GetFunction(context)
              .ToLocalChecked());

  exports->Set(context, Nan::New("process").ToLocalChecked(), obj);
}

#if NODE_MAJOR_VERSION >= 10
NAN_MODULE_WORKER_ENABLED(hello, Init)
#else
NODE_MODULE(hello, Init)
#endif