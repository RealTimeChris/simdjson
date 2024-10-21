#define SIMDJSON2_SRC_SIMDJSON2_CPP

#include <base.h>

SIMDJSON2_PUSH_DISABLE_UNUSED_WARNINGS

#include <to_chars.cpp>
#include <from_chars.cpp>
#include <internal/error_tables.cpp>
#include <internal/jsoncharutils_tables.cpp>
#include <internal/numberparsing_tables.cpp>
#include <internal/simdprune_tables.cpp>

#include <simdjson2/generic/dependencies.h>
#include <generic/dependencies.h>
#include <generic/stage1/dependencies.h>
#include <generic/stage2/dependencies.h>

#include <implementation.cpp>

#define SIMDJSON2_CONDITIONAL_INCLUDE

#if SIMDJSON2_IMPLEMENTATION_ARM64
#include <arm64.cpp>
#endif
#if SIMDJSON2_IMPLEMENTATION_HASWELL
#include <haswell.cpp>
#endif
#if SIMDJSON2_IMPLEMENTATION_ICELAKE
//#include <icelake.cpp>
#endif
#if SIMDJSON2_IMPLEMENTATION_PPC64
#include <ppc64.cpp>
#endif
#if SIMDJSON2_IMPLEMENTATION_WESTMERE
// #include <westmere.cpp>
#endif
#if SIMDJSON2_IMPLEMENTATION_LSX
#include <lsx.cpp>
#endif
#if SIMDJSON2_IMPLEMENTATION_LASX
#include <lasx.cpp>
#endif
#if SIMDJSON2_IMPLEMENTATION_FALLBACK
#include <fallback.cpp>
#endif
#undef SIMDJSON2_CONDITIONAL_INCLUDE

SIMDJSON2_POP_DISABLE_UNUSED_WARNINGS

