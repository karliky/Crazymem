{
  "targets": [
    {
      "target_name": "binding",
      "sources": [ "libmem/libmem.c", "libmem/libmem.h", "libmem.hpp", "bindings.cc" ],
      "include_dirs": [
        "<!(node -e \"require('nan')\")"
      ]
    }
  ]
}