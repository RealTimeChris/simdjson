#define SIMDJSON2_IMPLEMENTATION arm64
#include "simdjson2/arm64/base.h"
#include "simdjson2/arm64/intrinsics.h"
#include "simdjson2/arm64/bitmanipulation.h"
#include "simdjson2/arm64/bitmask.h"
#include "simdjson2/arm64/numberparsing_defs.h"
#include "simdjson2/arm64/simd.h"
#include "simdjson2/arm64/stringparsing_defs.h"

#define SIMDJSON2_SKIP_BACKSLASH_SHORT_CIRCUIT 1