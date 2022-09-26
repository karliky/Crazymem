export type WINDOW_NAME = string;
export type PROCESS_NAME = string;
export type PROCESS_PATH = string;
export type PID = number;
export type ADDRESS = number;
export type PROCESS_BITS = number;
export type PROCESS = {
    pid: string,
    handle: string
}
export type THREAD_ID = number;
export type ENUM_THREADS_CALLBACK = (tid: THREAD_ID) => Boolean;
export type MODULE = {
    base: ADDRESS,
    end: ADDRESS,
    size: ADDRESS
};
export type ENUM_MODULES_CALLBACK = (mod: MODULE, path: PROCESS_PATH) => Boolean;
export type MODULE_PATH = string;
export type MODULE_NAME = string;
export type DYNAMIC_LIBRARY_PATH = string;
export type ENUM_SYMBOLS_CALLBACK = (symbol: string, addr: ADDRESS) => Boolean;
export type SYM_STR = string;
export type MEMORY_PAGE = {
    base: ADDRESS,
    end: ADDRESS,
    size: ADDRESS,
    prot: ADDRESS,
    flags: ADDRESS
}
export type ENUM_MEMORY_PAGES_CALLBACK = (page: MEMORY_PAGE) => Boolean;
export type BSIZE = number;
export type MASK = string;