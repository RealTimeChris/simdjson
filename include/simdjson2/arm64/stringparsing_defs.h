#ifndef SIMDJSON2_ARM64_STRINGPARSING_DEFS_H
#define SIMDJSON2_ARM64_STRINGPARSING_DEFS_H

#ifndef SIMDJSON2_CONDITIONAL_INCLUDE
#include "simdjson2/arm64/base.h"
#include "simdjson2/arm64/simd.h"
#include "simdjson2/arm64/bitmanipulation.h"
#endif // SIMDJSON2_CONDITIONAL_INCLUDE

namespace simdjson2 {
namespace arm64 {
namespace {

using namespace simd;

// Holds backslashes and quotes locations.
struct backslash_and_quote {
public:
  static constexpr uint32_t BYTES_PROCESSED = 32;
  simdjson2_inline static backslash_and_quote copy_and_find(const uint8_t *src, uint8_t *dst);

  simdjson2_inline bool has_quote_first() { return ((bs_bits - 1) & quote_bits) != 0; }
  simdjson2_inline bool has_backslash() { return bs_bits != 0; }
  simdjson2_inline int quote_index() { return trailing_zeroes(quote_bits); }
  simdjson2_inline int backslash_index() { return trailing_zeroes(bs_bits); }

  uint32_t bs_bits;
  uint32_t quote_bits;
}; // struct backslash_and_quote

simdjson2_inline backslash_and_quote backslash_and_quote::copy_and_find(const uint8_t *src, uint8_t *dst) {
  // this can read up to 31 bytes beyond the buffer size, but we require
  // SIMDJSON2_PADDING of padding
  static_assert(SIMDJSON2_PADDING >= (BYTES_PROCESSED - 1), "backslash and quote finder must process fewer than SIMDJSON2_PADDING bytes");
  simd8<uint8_t> v0(src);
  simd8<uint8_t> v1(src + sizeof(v0));
  v0.store(dst);
  v1.store(dst + sizeof(v0));

  // Getting a 64-bit bitmask is much cheaper than multiple 16-bit bitmasks on ARM; therefore, we
  // smash them together into a 64-byte mask and get the bitmask from there.
  uint64_t bs_and_quote = simd8x64<bool>(v0 == '\\', v1 == '\\', v0 == '"', v1 == '"').to_bitmask();
  return {
    uint32_t(bs_and_quote),      // bs_bits
    uint32_t(bs_and_quote >> 32) // quote_bits
  };
}

} // unnamed namespace
} // namespace arm64
} // namespace simdjson2

#endif // SIMDJSON2_ARM64_STRINGPARSING_DEFS_H
