#ifndef SIMDJSON2_SRC_GENERIC_STAGE1_UTF8_VALIDATOR_H

#ifndef SIMDJSON2_CONDITIONAL_INCLUDE
#define SIMDJSON2_SRC_GENERIC_STAGE1_UTF8_VALIDATOR_H
#include <generic/stage1/base.h>
#include <generic/stage1/buf_block_reader.h>
#include <generic/stage1/utf8_lookup4_algorithm.h>
#endif // SIMDJSON2_CONDITIONAL_INCLUDE

namespace simdjson2 {
namespace SIMDJSON2_IMPLEMENTATION {
namespace {
namespace stage1 {

/**
 * Validates that the string is actual UTF-8.
 */
template<class checker>
bool generic_validate_utf8(const uint8_t * input, size_t length) {
    checker c{};
    buf_block_reader<256> reader(input, length);
    while (reader.has_full_block()) {
      simd::simd8x64<uint8_t> in(reader.full_block());
      c.check_next_input(in);
      reader.advance();
    }
    uint8_t block[64]{};
    reader.get_remainder(block);
    simd::simd8x64<uint8_t> in(block);
    c.check_next_input(in);
    reader.advance();
    c.check_eof();
    return c.errors() == error_code::SUCCESS;
}

bool generic_validate_utf8(const char * input, size_t length) {
    return generic_validate_utf8<utf8_checker>(reinterpret_cast<const uint8_t *>(input),length);
}

} // namespace stage1
} // unnamed namespace
} // namespace SIMDJSON2_IMPLEMENTATION
} // namespace simdjson2

#endif // SIMDJSON2_SRC_GENERIC_STAGE1_UTF8_VALIDATOR_H