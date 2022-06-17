{
  "targets": [
    {
      "target_name": "addon",
      "sources": [ "libmem\libmem\libmem.c", "libmem\libmem\libmem.h", "libmem\libmem\libmem.hpp", "./src/addon.cc" ],
      'include_dirs': ["<!(node -p \"require('node-addon-api').include_dir\")"],
      'defines': [ 'NAPI_DISABLE_CPP_EXCEPTIONS' ]
    }
  ]
}