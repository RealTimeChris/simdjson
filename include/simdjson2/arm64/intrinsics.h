#ifndef SIMDJSON2_ARM64_INTRINSICS_H
#define SIMDJSON2_ARM64_INTRINSICS_H

#ifndef SIMDJSON2_CONDITIONAL_INCLUDE
#include "simdjson2/arm64/base.h"
#endif // SIMDJSON2_CONDITIONAL_INCLUDE

// This should be the correct header whether
// you use visual studio or other compilers.
#include <arm_neon.h>

static_assert(sizeof(uint8x16_t) <= simdjson2::SIMDJSON2_PADDING, "insufficient padding for arm64");

#endif //  SIMDJSON2_ARM64_INTRINSICS_H
