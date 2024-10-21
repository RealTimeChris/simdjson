#ifndef SIMDJSON2_FALLBACK_STRINGPARSING_DEFS_H
#define SIMDJSON2_FALLBACK_STRINGPARSING_DEFS_H

#ifndef SIMDJSON2_CONDITIONAL_INCLUDE
#include "simdjson2/fallback/base.h"
#endif // SIMDJSON2_CONDITIONAL_INCLUDE

namespace simdjson2 {
namespace fallback {
namespace {

// Holds backslashes and quotes locations.
struct backslash_and_quote {
public:
  static constexpr uint32_t BYTES_PROCESSED = 1;
  simdjson2_inline static backslash_and_quote copy_and_find(const uint8_t *src, uint8_t *dst);

  simdjson2_inline bool has_quote_first() { return c == '"'; }
  simdjson2_inline bool has_backslash() { return c == '\\'; }
  simdjson2_inline int quote_index() { return c == '"' ? 0 : 1; }
  simdjson2_inline int backslash_index() { return c == '\\' ? 0 : 1; }

  uint8_t c;
}; // struct backslash_and_quote

simdjson2_inline backslash_and_quote backslash_and_quote::copy_and_find(const uint8_t *src, uint8_t *dst) {
  // store to dest unconditionally - we can overwrite the bits we don't like later
  dst[0] = src[0];
  return { src[0] };
}

} // unnamed namespace
} // namespace fallback
} // namespace simdjson2

#endif // SIMDJSON2_FALLBACK_STRINGPARSING_DEFS_H
