#ifndef SIMDJSON2_HASWELL_NUMBERPARSING_DEFS_H
#define SIMDJSON2_HASWELL_NUMBERPARSING_DEFS_H

#ifndef SIMDJSON2_CONDITIONAL_INCLUDE
#include "simdjson2/haswell/base.h"
#include "simdjson2/haswell/intrinsics.h"
#endif // SIMDJSON2_CONDITIONAL_INCLUDE

#ifndef SIMDJSON2_CONDITIONAL_INCLUDE
#include "simdjson2/internal/numberparsing_tables.h"
#endif // SIMDJSON2_CONDITIONAL_INCLUDE

namespace simdjson2 {
namespace haswell {
namespace numberparsing {

/** @private */
static simdjson2_inline uint32_t parse_eight_digits_unrolled(const uint8_t *chars) {
  // this actually computes *16* values so we are being wasteful.
  const __m128i ascii0 = _mm_set1_epi8('0');
  const __m128i mul_1_10 =
      _mm_setr_epi8(10, 1, 10, 1, 10, 1, 10, 1, 10, 1, 10, 1, 10, 1, 10, 1);
  const __m128i mul_1_100 = _mm_setr_epi16(100, 1, 100, 1, 100, 1, 100, 1);
  const __m128i mul_1_10000 =
      _mm_setr_epi16(10000, 1, 10000, 1, 10000, 1, 10000, 1);
  const __m128i input = _mm_sub_epi8(
      _mm_loadu_si128(reinterpret_cast<const __m128i *>(chars)), ascii0);
  const __m128i t1 = _mm_maddubs_epi16(input, mul_1_10);
  const __m128i t2 = _mm_madd_epi16(t1, mul_1_100);
  const __m128i t3 = _mm_packus_epi32(t2, t2);
  const __m128i t4 = _mm_madd_epi16(t3, mul_1_10000);
  return _mm_cvtsi128_si32(
      t4); // only captures the sum of the first 8 digits, drop the rest
}

/** @private */
simdjson2_inline internal::value128 full_multiplication(uint64_t value1, uint64_t value2) {
  internal::value128 answer;
#if SIMDJSON2_REGULAR_VISUAL_STUDIO || SIMDJSON2_IS_32BITS
#if SIMDJSON2_IS_ARM64
  // ARM64 has native support for 64-bit multiplications, no need to emultate
  answer.high = __umulh(value1, value2);
  answer.low = value1 * value2;
#else
  answer.low = _umul128(value1, value2, &answer.high); // _umul128 not available on ARM64
#endif // SIMDJSON2_IS_ARM64
#else // SIMDJSON2_REGULAR_VISUAL_STUDIO || SIMDJSON2_IS_32BITS
  __uint128_t r = (static_cast<__uint128_t>(value1)) * value2;
  answer.low = uint64_t(r);
  answer.high = uint64_t(r >> 64);
#endif
  return answer;
}

} // namespace numberparsing
} // namespace haswell
} // namespace simdjson2

#define SIMDJSON2_SWAR_NUMBER_PARSING 1

#endif // SIMDJSON2_HASWELL_NUMBERPARSING_DEFS_H
