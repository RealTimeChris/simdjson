#ifndef SIMDJSON2_HASWELL_STRINGPARSING_DEFS_H
#define SIMDJSON2_HASWELL_STRINGPARSING_DEFS_H

#ifndef SIMDJSON2_CONDITIONAL_INCLUDE
#include "simdjson2/haswell/base.h"
#include "simdjson2/haswell/simd.h"
#include "simdjson2/haswell/bitmanipulation.h"
#endif // SIMDJSON2_CONDITIONAL_INCLUDE

namespace simdjson2 {
namespace haswell {
namespace {

using namespace simd;

// Holds backslashes and quotes locations.
struct backslash_and_quote {
public:
  static constexpr uint32_t BYTES_PROCESSED = 32;
  simdjson2_inline static backslash_and_quote copy_and_find(const uint8_t *src, uint8_t *dst);

  simdjson2_inline bool has_quote_first() { return ((bs_bits - 1) & quote_bits) != 0; }
  simdjson2_inline bool has_backslash() { return ((quote_bits - 1) & bs_bits) != 0; }
  simdjson2_inline int quote_index() { return trailing_zeroes(quote_bits); }
  simdjson2_inline int backslash_index() { return trailing_zeroes(bs_bits); }

  uint32_t bs_bits;
  uint32_t quote_bits;
}; // struct backslash_and_quote

simdjson2_inline backslash_and_quote backslash_and_quote::copy_and_find(const uint8_t *src, uint8_t *dst) {
  // this can read up to 15 bytes beyond the buffer size, but we require
  // SIMDJSON2_PADDING of padding
  static_assert(SIMDJSON2_PADDING >= (BYTES_PROCESSED - 1), "backslash and quote finder must process fewer than SIMDJSON2_PADDING bytes");
  simd8<uint8_t> v(src);
  // store to dest unconditionally - we can overwrite the bits we don't like later
  v.store(dst);
  return {
      static_cast<uint32_t>((v == '\\').to_bitmask()),     // bs_bits
      static_cast<uint32_t>((v == '"').to_bitmask()), // quote_bits
  };
}

} // unnamed namespace
} // namespace haswell
} // namespace simdjson2

#endif // SIMDJSON2_HASWELL_STRINGPARSING_DEFS_H
