add_library(simdjson2-windows-headers INTERFACE)
if(MSVC OR MINGW)
  target_include_directories(simdjson2-windows-headers INTERFACE .)
  # getopt.h triggers bogus CRT_SECURE warnings. If you include them, you need this.
  target_compile_definitions(simdjson2-windows-headers INTERFACE _CRT_SECURE_NO_WARNINGS)
endif()
