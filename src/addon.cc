#include <assert.h>
#include <napi.h>
#include <stdint.h>
#include "../libmem/libmem/libmem.h"
#include <iostream>

static napi_value addon_LM_GetProcessIdEx(const Napi::CallbackInfo &info)
{
    auto env = info.Env();
    const char *procstr = info[0].ToString().Utf8Value().c_str();
    lm_pid_t result = LM_GetProcessIdEx((lm_tstring_t)procstr);

    napi_value rtn;
    napi_create_double(env, (double)result, &rtn);
    return rtn;
}

static napi_value addon_LM_GetParentIdEx(const Napi::CallbackInfo &info)
{
    auto env = info.Env();
    double pid = info[0].As<Napi::Number>().DoubleValue();
    lm_pid_t result = LM_GetParentIdEx((lm_pid_t)pid);

    napi_value rtn;
    napi_create_double(env, (double)result, &rtn);
    return rtn;
}

static napi_value addon_LM_OpenProcessEx(const Napi::CallbackInfo &info)
{
    auto env = info.Env();
    double pid = info[0].As<Napi::Number>().DoubleValue();
    lm_process_t proc;
    lm_bool_t result = LM_OpenProcessEx((lm_pid_t)pid, &proc);

    Napi::Object rtn = Napi::Object::New(env);
    rtn.Set("pid", proc.pid);
    rtn.Set("handle", (uintptr_t)proc.handle);

    return rtn;
}

static napi_value addon_LM_GetProcessPathEx(const Napi::CallbackInfo &info)
{
    auto env = info.Env();

    lm_process_t proc;
    lm_tchar_t procpath[LM_PATH_MAX];

    Napi::Object obj = info[0].As<Napi::Object>();
    auto pid = obj.Get("pid").ToNumber().DoubleValue();
    proc.pid = pid;
    auto handle = (uint64_t)obj.Get("handle").ToNumber().DoubleValue();
    proc.handle = (HANDLE)handle;
    LM_GetProcessPathEx(proc, procpath, LM_ARRLEN(procpath));
    std::string str(procpath);
    return Napi::String::New(env, str);
}

static napi_value addon_LM_GetProcessNameEx(const Napi::CallbackInfo &info)
{
    auto env = info.Env();
    lm_process_t proc;
    lm_tchar_t namebuf[LM_PATH_MAX];

    Napi::Object obj = info[0].As<Napi::Object>();
    auto pid = obj.Get("pid").ToNumber().DoubleValue();
    proc.pid = pid;
    auto handle = (uint64_t)obj.Get("handle").ToNumber().DoubleValue();
    proc.handle = (HANDLE)handle;

    LM_GetProcessNameEx(proc, namebuf, LM_ARRLEN(namebuf));
    std::string str(namebuf);
    return Napi::String::New(env, str);
}

static napi_value addon_LM_GetProcessBitsEx(const Napi::CallbackInfo &info)
{
    auto env = info.Env();
    lm_process_t proc;
    Napi::Object obj = info[0].As<Napi::Object>();
    auto pid = obj.Get("pid").ToNumber().DoubleValue();
    proc.pid = pid;
    auto handle = (uint64_t)obj.Get("handle").ToNumber().DoubleValue();
    proc.handle = (HANDLE)handle;
    napi_value rtn;
    napi_create_double(env, (double) LM_GetProcessBitsEx(proc), &rtn);
    return rtn;
}

lm_bool_t EnumThreadsExCallback(lm_tid_t tid, lm_void_t *funcPtr) {
    Napi::Function callback = *(Napi::Function*) funcPtr;
    napi_value val;
    napi_create_double(callback.Env(), tid, &val);
    auto result = callback.Call(std::initializer_list<napi_value>{val});
    return result.ToBoolean();
}

static napi_value addon_LM_EnumThreadsEx(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    lm_process_t proc;
    Napi::Object obj = info[0].As<Napi::Object>();
    auto pid = obj.Get("pid").ToNumber().DoubleValue();
    proc.pid = pid;
    auto handle = (uint64_t)obj.Get("handle").ToNumber().DoubleValue();
    proc.handle = (HANDLE)handle;

    Napi::Function callback = info[1].As<Napi::Function>();
    LM_EnumThreadsEx(proc, EnumThreadsExCallback, &callback);

    return env.Undefined();
}

static napi_value addon_LM_GetThreadIdEx(const Napi::CallbackInfo &info)
{
    auto env = info.Env();
    lm_process_t proc;
    Napi::Object obj = info[0].As<Napi::Object>();
    auto pid = obj.Get("pid").ToNumber().DoubleValue();
    proc.pid = pid;
    auto handle = (uint64_t)obj.Get("handle").ToNumber().DoubleValue();
    proc.handle = (HANDLE)handle;

    lm_pid_t result = LM_GetThreadIdEx(proc);

    napi_value rtn;
    napi_create_double(env, (double)result, &rtn);
    return rtn;
}

lm_bool_t EnumModulesExCallback(lm_module_t mod, lm_tstring_t path, lm_void_t  *funcPtr) {
    Napi::Function callback = *(Napi::Function*) funcPtr;

    Napi::Object obj = Napi::Object::New(callback.Env());
    obj.Set("base", (uintptr_t) mod.base);
    obj.Set("end", (uintptr_t) mod.end);
    obj.Set("size", (uintptr_t) mod.size);

    auto result = callback.Call(std::initializer_list<napi_value>{obj});
    return result.ToBoolean();
}

static napi_value addon_LM_EnumModulesEx(const Napi::CallbackInfo &info)
{
    auto env = info.Env();
    lm_process_t proc;
    Napi::Object obj = info[0].As<Napi::Object>();
    auto pid = obj.Get("pid").ToNumber().DoubleValue();
    proc.pid = pid;
    auto handle = (uint64_t)obj.Get("handle").ToNumber().DoubleValue();
    proc.handle = (HANDLE)handle;

    Napi::Function callback = info[1].As<Napi::Function>();
    LM_EnumModulesEx(proc, EnumModulesExCallback, &callback);

    return env.Undefined();
}

static napi_value addon_LM_GetModuleEx(const Napi::CallbackInfo &info)
{
    auto env = info.Env();
    lm_process_t proc;
    Napi::Object obj = info[0].As<Napi::Object>();
    auto pid = obj.Get("pid").ToNumber().DoubleValue();
    proc.pid = pid;
    auto handle = (uint64_t)obj.Get("handle").ToNumber().DoubleValue();
    proc.handle = (HANDLE)handle;

    const char *procpath = info[1].ToString().Utf8Value().c_str();

    lm_module_t mod;
    // TODO: Implement flags
    LM_GetModuleEx(proc, LM_MOD_BY_STR, (lm_tstring_t) procpath, &mod);

    Napi::Object rtn = Napi::Object::New(env);
    rtn.Set("base", (uintptr_t) mod.base);
    rtn.Set("end", (uintptr_t) mod.end);
    rtn.Set("size", (uintptr_t) mod.size);

    return rtn;
}

static napi_value addon_LM_GetModulePathEx(const Napi::CallbackInfo &info)
{
    auto env = info.Env();
    lm_process_t proc;
    lm_module_t mod;
    lm_tchar_t modpath[LM_PATH_MAX];

    Napi::Object obj = info[0].As<Napi::Object>();
    auto pid = obj.Get("pid").ToNumber().DoubleValue();
    proc.pid = pid;
    auto handle = (uint64_t)obj.Get("handle").ToNumber().DoubleValue();
    proc.handle = (HANDLE)handle;

    Napi::Object modJs = info[1].As<Napi::Object>();
    auto base = (uint64_t)modJs.Get("base").ToNumber().DoubleValue();
    mod.base = (lm_address_t) base;
    auto end = (uint64_t)modJs.Get("end").ToNumber().DoubleValue();
    mod.end = (lm_address_t) end;
    auto size = (uint64_t)modJs.Get("size").ToNumber().DoubleValue();
    mod.size = (lm_size_t) size;

    LM_GetModulePathEx(proc, mod, modpath, LM_ARRLEN(modpath));
    std::string str(modpath);
    return Napi::String::New(env, str);
}

static napi_value addon_LM_GetModuleNameEx(const Napi::CallbackInfo &info)
{
    auto env = info.Env();
    lm_process_t proc;
    lm_module_t mod;
    lm_tchar_t processPath[LM_PATH_MAX];

    Napi::Object obj = info[0].As<Napi::Object>();
    auto pid = obj.Get("pid").ToNumber().DoubleValue();
    proc.pid = pid;
    auto handle = (uint64_t)obj.Get("handle").ToNumber().DoubleValue();
    proc.handle = (HANDLE)handle;

    Napi::Object modJs = info[1].As<Napi::Object>();
    auto base = (uint64_t)modJs.Get("base").ToNumber().DoubleValue();
    mod.base = (lm_address_t) base;
    auto end = (uint64_t)modJs.Get("end").ToNumber().DoubleValue();
    mod.end = (lm_address_t) end;
    auto size = (uint64_t)modJs.Get("size").ToNumber().DoubleValue();
    mod.size = (lm_size_t) size;

    LM_GetModuleNameEx(proc, mod, processPath, LM_ARRLEN(processPath));
    std::string str(processPath);
    return Napi::String::New(env, str);
}

static napi_value addon_LM_LoadModuleEx(const Napi::CallbackInfo &info)
{
    auto env = info.Env();
    lm_module_t  load_lib;
    lm_process_t proc;

    Napi::Object obj = info[0].As<Napi::Object>();
    auto pid = obj.Get("pid").ToNumber().DoubleValue();
    proc.pid = pid;
    auto handle = (uint64_t)obj.Get("handle").ToNumber().DoubleValue();
    proc.handle = (HANDLE)handle;

    const char *procstr = info[1].ToString().Utf8Value().c_str();

    lm_bool_t result = LM_LoadModuleEx(proc, (lm_string_t) procstr, &load_lib);

    Napi::Object rtn = Napi::Object::New(env);
    rtn.Set("base", (uintptr_t) load_lib.base);
    rtn.Set("end", (uintptr_t) load_lib.end);
    rtn.Set("size", (uintptr_t) load_lib.size);
    return rtn;
}

static napi_value addon_LM_UnloadModuleEx(const Napi::CallbackInfo &info)
{
    auto env = info.Env();
    lm_process_t proc;
    Napi::Object obj = info[0].As<Napi::Object>();
    auto pid = obj.Get("pid").ToNumber().DoubleValue();
    proc.pid = pid;
    auto handle = (uint64_t)obj.Get("handle").ToNumber().DoubleValue();
    proc.handle = (HANDLE)handle;
    
    lm_module_t  mod;

    Napi::Object modJs = info[1].As<Napi::Object>();
    auto base = (uint64_t)modJs.Get("base").ToNumber().DoubleValue();
    mod.base = (lm_address_t) base;
    auto end = (uint64_t)modJs.Get("end").ToNumber().DoubleValue();
    mod.end = (lm_address_t) end;
    auto size = (uint64_t)modJs.Get("size").ToNumber().DoubleValue();
    mod.size = (lm_size_t) size;
    auto result = LM_UnloadModuleEx(proc, mod);
    return Napi::Boolean::New(env, result);
}


lm_bool_t EnumSymbolsExCallback(lm_cstring_t symbol, lm_address_t addr, lm_void_t *funcPtr) {
    std::cout << "EnumSymbolsExCallback\n";
    Napi::Function callback = *(Napi::Function*) funcPtr;
    std::cout << "symbol" << symbol << "addr" << addr << "\n";
    napi_value val;
    napi_create_string_utf8(callback.Env(), symbol, LM_ARRLEN(symbol), &val);
    auto result = callback.Call(std::initializer_list<napi_value>{val});
    return result.ToBoolean();
}

static napi_value addon_LM_EnumSymbolsEx(const Napi::CallbackInfo &info)
{
    auto env = info.Env();
    lm_process_t proc;
    Napi::Object obj = info[0].As<Napi::Object>();
    auto pid = obj.Get("pid").ToNumber().DoubleValue();
    proc.pid = pid;
    auto handle = (uint64_t)obj.Get("handle").ToNumber().DoubleValue();
    proc.handle = (HANDLE)handle;
    
    lm_module_t  mod;

    Napi::Object modJs = info[1].As<Napi::Object>();
    auto base = (uint64_t)modJs.Get("base").ToNumber().DoubleValue();
    mod.base = (lm_address_t) base;
    auto end = (uint64_t)modJs.Get("end").ToNumber().DoubleValue();
    mod.end = (lm_address_t) end;
    auto size = (uint64_t)modJs.Get("size").ToNumber().DoubleValue();
    mod.size = (lm_size_t) size;

    Napi::Function callback = info[2].As<Napi::Function>();
    lm_bool_t result = LM_EnumSymbolsEx(proc, mod , EnumSymbolsExCallback, &callback);
    // TODO: Research what's wrong with the internal alloc
    return Napi::String::New(env, "EnumSymbolsExCallback Not working properly");
}

static napi_value addon_LM_GetSymbolEx(const Napi::CallbackInfo &info)
{
    auto env = info.Env();
    lm_process_t proc;
    Napi::Object obj = info[0].As<Napi::Object>();
    auto pid = obj.Get("pid").ToNumber().DoubleValue();
    proc.pid = pid;
    auto handle = (uint64_t)obj.Get("handle").ToNumber().DoubleValue();
    proc.handle = (HANDLE)handle;
    
    lm_module_t  mod;

    Napi::Object modJs = info[1].As<Napi::Object>();
    auto base = (uint64_t)modJs.Get("base").ToNumber().DoubleValue();
    mod.base = (lm_address_t) base;
    auto end = (uint64_t)modJs.Get("end").ToNumber().DoubleValue();
    mod.end = (lm_address_t) end;
    auto size = (uint64_t)modJs.Get("size").ToNumber().DoubleValue();
    mod.size = (lm_size_t) size;

    const char *symstr = info[2].ToString().Utf8Value().c_str();

    lm_address_t result = LM_GetSymbolEx(proc, mod, (lm_cstring_t) symstr);

    napi_value rtn;
    napi_create_double(env, (uint64_t)result, &rtn);
    return rtn;
}

lm_bool_t EnumPagesExCallback(lm_page_t  page, lm_void_t  *funcPtr) {
    Napi::Function callback = *(Napi::Function*) funcPtr;

    Napi::Object obj = Napi::Object::New(callback.Env());
    obj.Set("base", (uintptr_t) page.base);
    obj.Set("end", (uintptr_t) page.end);
    obj.Set("size", (uintptr_t) page.size);
    obj.Set("prot", (uintptr_t) page.prot);
    obj.Set("flags", (uintptr_t) page.flags);

    auto result = callback.Call(std::initializer_list<napi_value>{obj});
    return result.ToBoolean();
}

static napi_value addon_LM_EnumPagesEx(const Napi::CallbackInfo &info)
{

    auto env = info.Env();
    lm_process_t proc;
    Napi::Object obj = info[0].As<Napi::Object>();
    auto pid = obj.Get("pid").ToNumber().DoubleValue();
    proc.pid = pid;
    auto handle = (uint64_t)obj.Get("handle").ToNumber().DoubleValue();
    proc.handle = (HANDLE)handle;

    Napi::Function callback = info[1].As<Napi::Function>();
    LM_EnumPagesEx(proc, EnumPagesExCallback, &callback);

    return env.Undefined();
}

static napi_value addon_LM_GetPageEx(const Napi::CallbackInfo &info)
{
    auto env = info.Env();
    lm_process_t proc;
    Napi::Object obj = info[0].As<Napi::Object>();
    auto pid = obj.Get("pid").ToNumber().DoubleValue();
    proc.pid = pid;
    auto handle = (uint64_t)obj.Get("handle").ToNumber().DoubleValue();
    proc.handle = (HANDLE)handle;
    lm_page_t page;
    
    auto addr = (uint64_t) info[1].As<Napi::Number>().DoubleValue();
    lm_bool_t result = LM_GetPageEx(proc, (lm_address_t) addr, &page);

    Napi::Object rtn = Napi::Object::New(env);
    rtn.Set("base", (uintptr_t) page.base);
    rtn.Set("end", (uintptr_t) page.end);
    rtn.Set("size", (uintptr_t) page.size);
    rtn.Set("prot", (uintptr_t) page.prot);
    rtn.Set("flags", (uintptr_t) page.flags);
    return rtn;
}

static napi_value addon_LM_ReadMemoryEx(const Napi::CallbackInfo &info)
{
    auto env = info.Env();
    lm_process_t proc;
    Napi::Object obj = info[0].As<Napi::Object>();
    auto pid = obj.Get("pid").ToNumber().DoubleValue();
    proc.pid = pid;
    auto handle = (uint64_t)obj.Get("handle").ToNumber().DoubleValue();
    proc.handle = (HANDLE)handle;

    auto src = info[1].As<Napi::Number>().Int64Value();
    auto size = info[2].As<Napi::Number>().DoubleValue();

    char* output_buffer = new char[size];
    LM_ReadMemoryEx(proc, (void*) src, (lm_byte_t *) output_buffer, (lm_size_t) size);
    Napi::Buffer<char> buffer = Napi::Buffer<char>::Copy(env, output_buffer, size);
    delete[] output_buffer;
    return buffer;
}

static napi_value addon_LM_WriteMemoryEx(const Napi::CallbackInfo &info)
{
    auto env = info.Env();
    lm_process_t proc;
    Napi::Object obj = info[0].As<Napi::Object>();
    auto pid = obj.Get("pid").ToNumber().DoubleValue();
    proc.pid = pid;
    auto handle = (uint64_t)obj.Get("handle").ToNumber().DoubleValue();
    proc.handle = (HANDLE)handle;

    auto address = info[1].As<Napi::Number>().Int64Value();
    Napi::Buffer<unsigned char> buffer = Napi::Buffer<unsigned char>(env, info[2]);
    lm_size_t result = LM_WriteMemoryEx(proc, (void*) address, (lm_bstring_t) buffer.Data(), buffer.Length());
    
    napi_value rtn;
    napi_create_double(env, (uint64_t)result, &rtn);
    return rtn;
}

static napi_value addon_LM_SetMemoryEx(const Napi::CallbackInfo &info)
{
    auto env = info.Env();
    lm_process_t proc;
    Napi::Object obj = info[0].As<Napi::Object>();
    auto pid = obj.Get("pid").ToNumber().DoubleValue();
    proc.pid = pid;
    auto handle = (uint64_t)obj.Get("handle").ToNumber().DoubleValue();
    proc.handle = (HANDLE)handle;

    auto dst = info[1].As<Napi::Number>().Int64Value();
    auto size = info[2].As<Napi::Number>().Int64Value();
    // TODO: Not sure about this byteFlag format
    auto byteFlag = info[3].As<Napi::Number>().Int64Value();

    lm_size_t result = LM_SetMemoryEx(proc, (void*) dst, (lm_byte_t) byteFlag, (lm_size_t) size);

    napi_value rtn;
    napi_create_double(env, (uint64_t)result, &rtn);
    return rtn;
}

static napi_value addon_LM_ProtMemoryEx(const Napi::CallbackInfo &info)
{
    auto env = info.Env();
    lm_process_t proc;
    // TODO: Not implemented
    //lm_bool_t result = LM_ProtMemoryEx(&proc, , LM_ARRLEN(procpath), , );

    return Napi::String::New(env, "Not implemented yet");
}

static napi_value addon_LM_AllocMemoryEx(const Napi::CallbackInfo &info)
{
    auto env = info.Env();
    lm_process_t proc;
    Napi::Object obj = info[0].As<Napi::Object>();
    auto pid = obj.Get("pid").ToNumber().DoubleValue();
    proc.pid = pid;
    auto handle = (uint64_t)obj.Get("handle").ToNumber().DoubleValue();
    proc.handle = (HANDLE)handle;

    auto size = info[1].As<Napi::Number>().Int64Value();
    auto protection = info[2].As<Napi::Number>().Int64Value();

    lm_address_t result = LM_AllocMemoryEx(proc, size, protection);

    napi_value rtn;
    napi_create_double(env, (uint64_t)result, &rtn);
    return rtn;
}

static napi_value addon_LM_FreeMemoryEx(const Napi::CallbackInfo &info)
{
    auto env = info.Env();
    lm_process_t proc;
    Napi::Object obj = info[0].As<Napi::Object>();
    auto pid = obj.Get("pid").ToNumber().DoubleValue();
    proc.pid = pid;
    auto handle = (uint64_t)obj.Get("handle").ToNumber().DoubleValue();
    proc.handle = (HANDLE)handle;

    auto dst = info[1].As<Napi::Number>().Int64Value();
    auto size = info[2].As<Napi::Number>().Int64Value();

    lm_bool_t result = LM_FreeMemoryEx(proc, (void*) dst, size);

    napi_value rtn;
    napi_create_double(env, result, &rtn);
    return rtn;
}

static napi_value addon_LM_DataScanEx(const Napi::CallbackInfo &info)
{
    auto env = info.Env();
    lm_process_t proc;
    Napi::Object obj = info[0].As<Napi::Object>();
    auto pid = obj.Get("pid").ToNumber().DoubleValue();
    proc.pid = pid;
    auto handle = (uint64_t)obj.Get("handle").ToNumber().DoubleValue();
    proc.handle = (HANDLE)handle;

    auto data = Napi::Buffer<unsigned char>(env, info[1]);
    auto size = info[2].As<Napi::Number>().Int64Value();
    auto addr = info[3].As<Napi::Number>().Int64Value();
    auto scansize = info[4].As<Napi::Number>().Int64Value();

    lm_address_t result = LM_DataScanEx(proc, (lm_bstring_t) data.Data(), size, (void*) addr, scansize);

    napi_value rtn;
    napi_create_double(env, (int64_t) result, &rtn);
    return rtn;
}

static napi_value addon_LM_PatternScanEx(const Napi::CallbackInfo &info)
{
    auto env = info.Env();
    lm_process_t proc;
    Napi::Object obj = info[0].As<Napi::Object>();
    auto pid = obj.Get("pid").ToNumber().DoubleValue();
    proc.pid = pid;
    auto handle = (uint64_t)obj.Get("handle").ToNumber().DoubleValue();
    proc.handle = (HANDLE)handle;

    auto data = Napi::Buffer<unsigned char>(env, info[1]);
    std::string mask = info[2].ToString().Utf8Value();
    auto addr = info[3].As<Napi::Number>().Int64Value();
    auto scansize = info[4].As<Napi::Number>().Int64Value();

    const char *mask_str = mask.c_str();

    lm_address_t result = LM_PatternScanEx(proc, (lm_bstring_t) data.Data(), (lm_tstring_t) mask_str, (void*) addr, scansize);

    napi_value rtn;
    napi_create_double(env, (int64_t) result, &rtn);
    return rtn;
}

static napi_value addon_LM_SigScanEx(const Napi::CallbackInfo &info)
{
    auto env = info.Env();
    lm_process_t proc;
    Napi::Object obj = info[0].As<Napi::Object>();
    auto pid = obj.Get("pid").ToNumber().DoubleValue();
    proc.pid = pid;
    auto handle = (uint64_t)obj.Get("handle").ToNumber().DoubleValue();
    proc.handle = (HANDLE)handle;
    
    std::string sig = info[1].ToString().Utf8Value();
    auto addr = info[2].As<Napi::Number>().Int64Value();
    auto scansize = info[3].As<Napi::Number>().Int64Value();

    const char *sig_str = sig.c_str();
    lm_address_t result = LM_SigScanEx(proc, (lm_tstring_t) sig_str,  (void*) addr, scansize);

    napi_value rtn;
    napi_create_double(env, (int64_t) result, &rtn);
    return rtn;
}

static napi_value addon_LM_SystemCallEx(const Napi::CallbackInfo &info)
{
    auto env = info.Env();
    lm_process_t proc;
    Napi::Object obj = info[0].As<Napi::Object>();
    auto pid = obj.Get("pid").ToNumber().DoubleValue();
    proc.pid = pid;
    auto handle = (uint64_t)obj.Get("handle").ToNumber().DoubleValue();
    proc.handle = (HANDLE)handle;

    auto stack_align = info[1].As<Napi::Number>().Int64Value();
    auto nargs = info[2].As<Napi::Number>().Int64Value();
    auto nrets = info[3].As<Napi::Number>().Int64Value();


    lm_bool_t result = LM_SystemCallEx(proc, stack_align, nargs,  nrets);

    napi_value rtn;
    napi_create_double(env, (int64_t) result, &rtn);
    return rtn;
}

static napi_value addon_LM_FunctionCallEx(const Napi::CallbackInfo &info)
{
    auto env = info.Env();
    lm_process_t proc;
    Napi::Object obj = info[0].As<Napi::Object>();
    auto pid = obj.Get("pid").ToNumber().DoubleValue();
    proc.pid = pid;
    auto handle = (uint64_t)obj.Get("handle").ToNumber().DoubleValue();
    proc.handle = (HANDLE)handle;

    auto stack_align = info[1].As<Napi::Number>().Int64Value();
    auto fnaddr = info[2].As<Napi::Number>().Int64Value();
    auto nargs = info[3].As<Napi::Number>().Int64Value();
    auto nrets = info[4].As<Napi::Number>().Int64Value();
    lm_bool_t result = LM_FunctionCallEx(proc, stack_align, (lm_address_t) fnaddr, nargs,  nrets);

    napi_value rtn;
    napi_create_double(env, (int64_t) result, &rtn);
    return rtn;
}

static napi_value addon_LM_DetourCodeEx(const Napi::CallbackInfo &info)
{
    auto env = info.Env();
    lm_process_t proc;
    Napi::Object obj = info[0].As<Napi::Object>();
    auto pid = obj.Get("pid").ToNumber().DoubleValue();
    proc.pid = pid;
    auto handle = (uint64_t)obj.Get("handle").ToNumber().DoubleValue();
    proc.handle = (HANDLE)handle;

    auto src = info[1].As<Napi::Number>().Int64Value();
    auto dst = info[2].As<Napi::Number>().Int64Value();
    auto detour = info[3].As<Napi::Number>().Int64Value();

    lm_bool_t result = LM_DetourCodeEx(proc, (lm_address_t) src, (lm_address_t) dst, detour);

    napi_value rtn;
    napi_create_double(env, (int64_t) result, &rtn);
    return rtn;
}

static napi_value addon_LM_MakeTrampolineEx(const Napi::CallbackInfo &info)
{
    auto env = info.Env();
    lm_process_t proc;
    Napi::Object obj = info[0].As<Napi::Object>();
    auto pid = obj.Get("pid").ToNumber().DoubleValue();
    proc.pid = pid;
    auto handle = (uint64_t)obj.Get("handle").ToNumber().DoubleValue();
    proc.handle = (HANDLE)handle;

    auto src = info[1].As<Napi::Number>().Int64Value();
    auto size = info[2].As<Napi::Number>().Int64Value();

    lm_address_t result = LM_MakeTrampolineEx(proc, (lm_address_t) src, size);

    napi_value rtn;
    napi_create_double(env, (int64_t) result, &rtn);
    return rtn;
}

static napi_value addon_LM_DestroyTrampolineEx(const Napi::CallbackInfo &info)
{
    auto env = info.Env();
    lm_process_t proc;
    Napi::Object obj = info[0].As<Napi::Object>();
    auto pid = obj.Get("pid").ToNumber().DoubleValue();
    proc.pid = pid;
    auto handle = (uint64_t)obj.Get("handle").ToNumber().DoubleValue();
    proc.handle = (HANDLE)handle;

    auto tramp = info[1].As<Napi::Number>().Int64Value();

    LM_DestroyTrampolineEx(proc, (lm_address_t) tramp);

    napi_value rtn;
    napi_create_double(env, 1, &rtn);
    return rtn;
}

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
    exports.Set(Napi::String::New(env, "LM_GetProcessIdEx"), Napi::Function::New(env, addon_LM_GetProcessIdEx));
    exports.Set(Napi::String::New(env, "LM_GetParentIdEx"), Napi::Function::New(env, addon_LM_GetParentIdEx));
    exports.Set(Napi::String::New(env, "LM_OpenProcessEx"), Napi::Function::New(env, addon_LM_OpenProcessEx));
    exports.Set(Napi::String::New(env, "LM_GetProcessPathEx"), Napi::Function::New(env, addon_LM_GetProcessPathEx));
    exports.Set(Napi::String::New(env, "LM_GetProcessNameEx"), Napi::Function::New(env, addon_LM_GetProcessNameEx));
    exports.Set(Napi::String::New(env, "LM_GetProcessBitsEx"), Napi::Function::New(env, addon_LM_GetProcessBitsEx));
    exports.Set(Napi::String::New(env, "LM_EnumThreadsEx"), Napi::Function::New(env, addon_LM_EnumThreadsEx));
    exports.Set(Napi::String::New(env, "LM_GetThreadIdEx"), Napi::Function::New(env, addon_LM_GetThreadIdEx));
    exports.Set(Napi::String::New(env, "LM_EnumModulesEx"), Napi::Function::New(env, addon_LM_EnumModulesEx));
    exports.Set(Napi::String::New(env, "LM_GetModuleEx"), Napi::Function::New(env, addon_LM_GetModuleEx));
    exports.Set(Napi::String::New(env, "LM_GetModulePathEx"), Napi::Function::New(env, addon_LM_GetModulePathEx));
    exports.Set(Napi::String::New(env, "LM_GetModuleNameEx"), Napi::Function::New(env, addon_LM_GetModuleNameEx));
    exports.Set(Napi::String::New(env, "LM_LoadModuleEx"), Napi::Function::New(env, addon_LM_LoadModuleEx));
    exports.Set(Napi::String::New(env, "LM_UnloadModuleEx"), Napi::Function::New(env, addon_LM_UnloadModuleEx));
    exports.Set(Napi::String::New(env, "LM_EnumSymbolsEx"), Napi::Function::New(env, addon_LM_EnumSymbolsEx));
    exports.Set(Napi::String::New(env, "LM_GetSymbolEx"), Napi::Function::New(env, addon_LM_GetSymbolEx));
    exports.Set(Napi::String::New(env, "LM_EnumPagesEx"), Napi::Function::New(env, addon_LM_EnumPagesEx));
    exports.Set(Napi::String::New(env, "LM_GetPageEx"), Napi::Function::New(env, addon_LM_GetPageEx));
    exports.Set(Napi::String::New(env, "LM_ReadMemoryEx"), Napi::Function::New(env, addon_LM_ReadMemoryEx));
    exports.Set(Napi::String::New(env, "LM_WriteMemoryEx"), Napi::Function::New(env, addon_LM_WriteMemoryEx));
    exports.Set(Napi::String::New(env, "LM_SetMemoryEx"), Napi::Function::New(env, addon_LM_SetMemoryEx));
    exports.Set(Napi::String::New(env, "LM_ProtMemoryEx"), Napi::Function::New(env, addon_LM_ProtMemoryEx));
    exports.Set(Napi::String::New(env, "LM_AllocMemoryEx"), Napi::Function::New(env, addon_LM_AllocMemoryEx));
    exports.Set(Napi::String::New(env, "LM_FreeMemoryEx"), Napi::Function::New(env, addon_LM_FreeMemoryEx));
    exports.Set(Napi::String::New(env, "LM_DataScanEx"), Napi::Function::New(env, addon_LM_DataScanEx));
    exports.Set(Napi::String::New(env, "LM_PatternScanEx"), Napi::Function::New(env, addon_LM_PatternScanEx));
    exports.Set(Napi::String::New(env, "LM_SigScanEx"), Napi::Function::New(env, addon_LM_SigScanEx));
    exports.Set(Napi::String::New(env, "LM_SystemCallEx"), Napi::Function::New(env, addon_LM_SystemCallEx));
    exports.Set(Napi::String::New(env, "LM_FunctionCallEx"), Napi::Function::New(env, addon_LM_FunctionCallEx));
    exports.Set(Napi::String::New(env, "LM_DetourCodeEx"), Napi::Function::New(env, addon_LM_DetourCodeEx));
    exports.Set(Napi::String::New(env, "LM_MakeTrampolineEx"), Napi::Function::New(env, addon_LM_MakeTrampolineEx));
    exports.Set(Napi::String::New(env, "LM_DestroyTrampolineEx"), Napi::Function::New(env, addon_LM_DestroyTrampolineEx));
    return exports;
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, Init)