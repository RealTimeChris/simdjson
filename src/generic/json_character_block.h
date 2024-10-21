#ifndef SIMDJSON2_SRC_GENERIC_JSON_CHARACTER_BLOCK_H

#ifndef SIMDJSON2_CONDITIONAL_INCLUDE
#define SIMDJSON2_SRC_GENERIC_JSON_CHARACTER_BLOCK_H
#include <generic/base.h>
#endif // SIMDJSON2_CONDITIONAL_INCLUDE

namespace simdjson2 {
namespace SIMDJSON2_IMPLEMENTATION {
namespace {

struct json_character_block {
  static simdjson2_inline json_character_block
  classify(const simd::simd8<uint8_t> (&in)[8]);

  simdjson2_inline simd::simd8<uint8_t> whitespace() const noexcept {
    return _whitespace;
  }
  simdjson2_inline simd::simd8<uint8_t> op() const noexcept { return _op; }
  simdjson2_inline simd::simd8<uint8_t> scalar() const noexcept {
    return ~(op() | whitespace());
  }

  simd::simd8<uint8_t> _whitespace;
  simd::simd8<uint8_t> _op;
};

} // unnamed namespace
} // namespace SIMDJSON2_IMPLEMENTATION
} // namespace simdjson2

#endif // SIMDJSON2_SRC_GENERIC_JSON_CHARACTER_BLOCK_H
