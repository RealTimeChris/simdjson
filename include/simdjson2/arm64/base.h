#ifndef SIMDJSON2_ARM64_BASE_H
#define SIMDJSON2_ARM64_BASE_H

#ifndef SIMDJSON2_CONDITIONAL_INCLUDE
#include "simdjson2/base.h"
#endif // SIMDJSON2_CONDITIONAL_INCLUDE

namespace simdjson2 {
/**
 * Implementation for NEON (ARMv8).
 */
namespace arm64 {

class implementation;

namespace {
namespace simd {
template <typename T> struct simd8;
template <typename T> struct simd8x64;
} // namespace simd
} // unnamed namespace

} // namespace arm64
} // namespace simdjson2

#endif // SIMDJSON2_ARM64_BASE_H
