#ifndef SIMDJSON2_HASWELL_BITMANIPULATION_H
#define SIMDJSON2_HASWELL_BITMANIPULATION_H

#ifndef SIMDJSON2_CONDITIONAL_INCLUDE
#include "simdjson2/haswell/base.h"
#include "simdjson2/haswell/intrinsics.h"
#include "simdjson2/haswell/bitmask.h"
#endif // SIMDJSON2_CONDITIONAL_INCLUDE

namespace simdjson2 {
namespace haswell {
namespace {

// We sometimes call trailing_zero on inputs that are zero,
// but the algorithms do not end up using the returned value.
// Sadly, sanitizers are not smart enough to figure it out.
SIMDJSON2_NO_SANITIZE_UNDEFINED
// This function can be used safely even if not all bytes have been
// initialized.
// See issue https://github.com/simdjson2/simdjson2/issues/1965
SIMDJSON2_NO_SANITIZE_MEMORY
simdjson2_inline int trailing_zeroes(uint64_t input_num) {
#if SIMDJSON2_REGULAR_VISUAL_STUDIO
  return (int)_tzcnt_u64(input_num);
#else // SIMDJSON2_REGULAR_VISUAL_STUDIO
  ////////
  // You might expect the next line to be equivalent to
  // return (int)_tzcnt_u64(input_num);
  // but the generated code differs and might be less efficient?
  ////////
  return __builtin_ctzll(input_num);
#endif // SIMDJSON2_REGULAR_VISUAL_STUDIO
}

/* result might be undefined when input_num is zero */
simdjson2_inline uint64_t clear_lowest_bit(uint64_t input_num) {
  return _blsr_u64(input_num);
}

/* result might be undefined when input_num is zero */
simdjson2_inline int leading_zeroes(uint64_t input_num) {
  return int(_lzcnt_u64(input_num));
}

#if SIMDJSON2_REGULAR_VISUAL_STUDIO
simdjson2_inline unsigned __int64 count_ones(uint64_t input_num) {
  // note: we do not support legacy 32-bit Windows in this kernel
  return __popcnt64(input_num);// Visual Studio wants two underscores
}
#else
simdjson2_inline long long int count_ones(uint64_t input_num) {
  return _popcnt64(input_num);
}
#endif

simdjson2_inline bool add_overflow(uint64_t value1, uint64_t value2,
                                uint64_t *result) {
#if SIMDJSON2_REGULAR_VISUAL_STUDIO
  return _addcarry_u64(0, value1, value2,
                       reinterpret_cast<unsigned __int64 *>(result));
#else
  return __builtin_uaddll_overflow(value1, value2,
                                   reinterpret_cast<unsigned long long *>(result));
#endif
}

} // unnamed namespace
} // namespace haswell
} // namespace simdjson2

#endif // SIMDJSON2_HASWELL_BITMANIPULATION_H
