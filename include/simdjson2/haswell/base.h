#ifndef SIMDJSON2_HASWELL_BASE_H
#define SIMDJSON2_HASWELL_BASE_H

#ifndef SIMDJSON2_CONDITIONAL_INCLUDE
#include "simdjson2/base.h"
#endif // SIMDJSON2_CONDITIONAL_INCLUDE

// The constructor may be executed on any host, so we take care not to use SIMDJSON2_TARGET_HASWELL
namespace simdjson2 {
/**
 * Implementation for Haswell (Intel AVX2).
 */
namespace haswell {

class implementation;

namespace {
namespace simd {
template <typename T> struct simd8;
template <typename T> struct simd8x64;
} // namespace simd
} // unnamed namespace

} // namespace haswell
} // namespace simdjson2

#endif // SIMDJSON2_HASWELL_BASE_H
