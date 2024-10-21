#define SIMDJSON2_IMPLEMENTATION haswell

#include "simdjson2/haswell/base.h"
#include "simdjson2/haswell/intrinsics.h"

#if !SIMDJSON2_CAN_ALWAYS_RUN_HASWELL
// We enable bmi2 only if LLVM/clang is used, because GCC may not
// make good use of it. See https://github.com/simdjson2/simdjson2/pull/2243
#if defined(__clang__)
SIMDJSON2_TARGET_REGION("avx2,bmi,bmi2,pclmul,lzcnt,popcnt")
#else
SIMDJSON2_TARGET_REGION("avx2,bmi,pclmul,lzcnt,popcnt")
#endif
#endif

#include "simdjson2/haswell/bitmanipulation.h"
#include "simdjson2/haswell/bitmask.h"
#include "simdjson2/haswell/numberparsing_defs.h"
#include "simdjson2/haswell/simd.h"
#include "simdjson2/haswell/stringparsing_defs.h"
