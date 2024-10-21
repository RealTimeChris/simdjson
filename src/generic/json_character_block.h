#ifndef SIMDJSON_SRC_GENERIC_JSON_CHARACTER_BLOCK_H

#ifndef SIMDJSON_CONDITIONAL_INCLUDE
#define SIMDJSON_SRC_GENERIC_JSON_CHARACTER_BLOCK_H
#include <generic/base.h>
#endif // SIMDJSON_CONDITIONAL_INCLUDE

namespace simdjson {
namespace SIMDJSON_IMPLEMENTATION {
namespace {

struct json_character_block {
  static simdjson_inline json_character_block
  classify(const simd::simd8x64<uint8_t> (&in)[4]);

  simdjson_inline simd::simd8<uint8_t> whitespace() const noexcept {
    return _whitespace;
  }
  simdjson_inline simd::simd8<uint8_t> op() const noexcept { return _op; }
  simdjson_inline simd::simd8<uint8_t> scalar() const noexcept {
    return ~(op() | whitespace());
  }

  simd::simd8<uint8_t> _whitespace;
  simd::simd8<uint8_t> _op;
};

} // unnamed namespace
} // namespace SIMDJSON_IMPLEMENTATION
} // namespace simdjson

#endif // SIMDJSON_SRC_GENERIC_JSON_CHARACTER_BLOCK_H
