#ifndef SIMDJSON2_SRC_GENERIC_DOM_PARSER_IMPLEMENTATION_H

#ifndef SIMDJSON2_CONDITIONAL_INCLUDE
#define SIMDJSON2_SRC_GENERIC_DOM_PARSER_IMPLEMENTATION_H
#include <generic/base.h>
#include <simdjson2/generic/dom_parser_implementation.h>
#endif // SIMDJSON2_CONDITIONAL_INCLUDE

// Interface a dom parser implementation must fulfill
namespace simdjson2 {
namespace SIMDJSON2_IMPLEMENTATION {
namespace {

simdjson2_inline simd8<uint8_t> must_be_2_3_continuation(const simd8<uint8_t> prev2, const simd8<uint8_t> prev3);
simdjson2_inline bool is_ascii(const simd8x64<uint8_t>& input);

} // unnamed namespace
} // namespace SIMDJSON2_IMPLEMENTATION
} // namespace simdjson2

#endif // SIMDJSON2_SRC_GENERIC_DOM_PARSER_IMPLEMENTATION_H
