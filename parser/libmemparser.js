const { readFileSync, writeFileSync } = require(`fs`);

const libMem = readFileSync("./libmem/libmem/libmem.h", { encoding: 'utf-8' });

const methods = [...libMem.matchAll(new RegExp("LM_API ", 'gi'))].map(a => a.index);

const MAXIMUM_FN_BODY_LENGTH = 1000;
const methodsDescription = methods.map((index) => {
    const api = libMem.substr(index, MAXIMUM_FN_BODY_LENGTH);
    const returnTypeMatch = api.match(" (.*)_t");
    if(!returnTypeMatch) return;
    const functionMatch = (api.substring(api.indexOf('_t')+1,api.indexOf(");")) + ");");
    const name = "LM_" + (functionMatch.split("LM_")[1].split("(")[0]);
    const returnType = returnTypeMatch[0].replace(" ", "");
    const arguments = (functionMatch.split("(")[1].split(")")[0]).split(",").map(str => str.replace(/[\r\n\t]/gm, '').trim());
    if (!name.includes("Ex")) return;
    return {
        name: name,
        return: returnType,
        arguments
    }
}).filter((method) => !!method);

const buildArguments = (args) => {
    return args.map((arg) => {
        const definition = arg.split(" ").filter(n => n);
        const type = definition[0];
        const name = definition[1];
        return { type, name };
    });
}

const buildStringArgument = (index, arg) => {
    return {
        variable: `const char* ${arg.name} = info[${index}].ToString().Utf8Value().c_str();`,
        argument: `(${arg.type}) ${arg.name}`
    };
}

const buildArgumentsAsDouble = (index, arg) => {
    return {
        variable: `double ${arg.name} = info[${index}].As<Napi::Number>().DoubleValue();`,
        argument: `(${arg.type}) ${arg.name}`
    };
}

const buildArgumentAsProc = () => {
    return {
        variable: `lm_process_t proc;`,
        argument: `&proc`
    };
}

const buildArgumentAsChar = (arg) => {
    return {
        variable: `lm_tchar_t procpath[LM_PATH_MAX];
        
        Napi::Object obj = info[0].As<Napi::Object>();
        auto pid = obj.Get("pid").ToNumber().DoubleValue();
        proc.pid = pid;
        auto handle = (uint64_t) obj.Get("handle").ToNumber().DoubleValue();
        proc.handle = (HANDLE) handle;
        `,
        argument: `procpath`
    };
}

const buildArgumentAsCharSize = (arg) => {
    return {
        variable: ``,
        argument: `LM_ARRLEN(procpath)`
    };
}

const buildArgumentsAsCpp = (args) => {
    return args.map((arg, index) => {
        if (arg.type === "lm_tstring_t") return buildStringArgument(index, arg);
        if (arg.type === "lm_pid_t") return buildArgumentsAsDouble(index, arg);
        if (arg.type === "lm_process_t") return buildArgumentAsProc();
        if (arg.type === "lm_tchar_t") return buildArgumentAsChar(arg);
        if (arg.type === "lm_size_t") return buildArgumentAsCharSize(arg);
        
        return { variable: "", argument: "" }
    });
}

const buildRtnAsCpp = (returnType) => {
    if (returnType === "lm_pid_t") return `
    napi_value rtn;
    napi_create_double(env, (double) result, &rtn);`
    if (returnType['name'] && returnType['type'] === "lm_process_t") return `
    Napi::Object rtn = Napi::Object::New(env);
    rtn.Set("pid", proc.pid);
    rtn.Set("handle", (uintptr_t)proc.handle);
    `
    return ``;
}

const buildReturnValue = (returnType, arguments) => {
    const returnAsPtr = arguments.find((arg) => arg.name && arg.name.includes('*'));
    return `
    ${buildRtnAsCpp(returnAsPtr || returnType)}
    return rtn;
    `;
}

const nativeFunctions = methodsDescription.map((fn) => {
    const args = buildArguments(fn.arguments);
    const argsAsCppCode = buildArgumentsAsCpp(args);
    const rtnValue = buildReturnValue(fn.return, args);
    return `
    static napi_value addon_${fn.name}(const Napi::CallbackInfo& info)
    {
        auto env = info.Env();
        ${argsAsCppCode.map((definition) => definition.variable).join("\n")}
        ${fn.return} result = ${fn.name}(${argsAsCppCode.map((definition) => definition.argument).join(",")});
        ${rtnValue}
    }  
    `;
});

const nativeExports = methodsDescription.map((fn) => {
    return `exports.Set(Napi::String::New(env, "${fn.name}"), Napi::Function::New(env, addon_${fn.name}));\n`;
});

var template = readFileSync('./src/napi-template', 'utf-8');
template = template.replace("#functions#", nativeFunctions.join(""));
template = template.replace("#functions_export#", nativeExports.join(""));
writeFileSync('./src/addon_template.cc', template);
