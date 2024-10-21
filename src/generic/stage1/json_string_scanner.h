#ifndef SIMDJSON2_SRC_GENERIC_STAGE1_JSON_STRING_SCANNER_H
#include <bitset>
#ifndef SIMDJSON2_CONDITIONAL_INCLUDE
#define SIMDJSON2_SRC_GENERIC_STAGE1_JSON_STRING_SCANNER_H
#include <generic/stage1/base.h>
#include <generic/stage1/json_escape_scanner.h>
#endif // SIMDJSON2_CONDITIONAL_INCLUDE

namespace simdjson2 {
namespace SIMDJSON2_IMPLEMENTATION {
namespace {
namespace stage1 {

struct json_string_block {
  // We spell out the constructors in the hope of resolving inlining issues with Visual Studio 2017
  simdjson2_really_inline json_string_block(const simd::simd8<uint8_t>& escaped, const simd::simd8<uint8_t>& quote, const simd::simd8<uint8_t>& in_string) :
  _escaped(escaped), _quote(quote), _in_string(in_string) {}

  // Escaped characters (characters following an escape() character)
  simdjson2_really_inline simd::simd8<uint8_t> escaped() const { return _escaped; }
  // Real (non-backslashed) quotes
  simdjson2_really_inline simd::simd8<uint8_t> quote() const { return _quote; }
  // Only characters inside the string (not including the quotes)
  simdjson2_really_inline simd::simd8<uint8_t> string_content() const { return _in_string.bit_andnot(_quote); }
  // Return a mask of whether the given characters are inside a string (only works on non-quotes)
  simdjson2_really_inline simd::simd8<uint8_t> non_quote_inside_string(const simd::simd8<uint8_t>& mask) const { return mask.bit_andnot(_in_string); }
  // Return a mask of whether the given characters are inside a string (only works on non-quotes)
  simdjson2_really_inline simd::simd8<uint8_t>
  non_quote_outside_string(const simd::simd8<uint8_t>& mask) const {
    return mask.bit_andnot(_in_string);
  }
  // Tail of string (everything except the start quote)
  simdjson2_really_inline simd::simd8<uint8_t> string_tail() const { return _in_string ^ _quote; }

  // escaped characters (backslashed--does not include the hex characters after \u)
  simd::simd8<uint8_t> _escaped;
  // real quotes (non-escaped ones)
  simd::simd8<uint8_t> _quote;
  // string characters (includes start quote but not end quote)
  simd::simd8<uint8_t> _in_string;
};

// Scans blocks for string characters, storing the state necessary to do so
class json_string_scanner {
public:
  simdjson2_really_inline json_string_block
  next(const simd::simd8<uint8_t> (&in)[8]);
  // Returns either UNCLOSED_STRING or SUCCESS
  simdjson2_really_inline error_code finish();

private:
  // Scans for escape characters
  json_escape_scanner escape_scanner{};
  // Whether the last iteration was still inside a string (all 1's = true, all 0's = false).
  uint64_t prev_in_string{};
};

simdjson2_really_inline static uint64_t prefixXor(uint64_t prevInString) noexcept {
  prevInString ^= prevInString << 1;
  prevInString ^= prevInString << 2;
  prevInString ^= prevInString << 4;
  prevInString ^= prevInString << 8;
  prevInString ^= prevInString << 16;
  prevInString ^= prevInString << 32;
  return prevInString;
}

//
// Return a mask of all string characters plus end quotes.
//
// prev_escaped is overflow saying whether the next character is escaped.
// prev_in_string is overflow saying whether we're still in a string.
//
// Backslash sequences outside of quotes will be detected in stage 2.
//
simdjson2_really_inline json_string_block
json_string_scanner::next(const simd::simd8<uint8_t> (&in)[8]) {
  alignas(32) uint32_t backslash_pre[8];
  alignas(32) uint32_t escaped_pre[8];
  alignas(32) uint32_t quote_pre[8];
  for (size_t x = 0; x < 8; ++x) {
    backslash_pre[x] = in[x].eq('\\');
    quote_pre[x] = in[x].eq('"');
  }

  const simd::simd8<uint8_t> backslash{
      simd::simd8<uint8_t>::load(backslash_pre)};
  const simd::simd8<uint8_t> escaped = escape_scanner.next(backslash).escaped;
  simd::simd8<uint8_t> quote{simd::simd8<uint8_t>::load(quote_pre)};
  quote = quote.bit_andnot(escaped);
  
  alignas(32) uint64_t valuesIn[4];
  quote.store(valuesIn);
  valuesIn[0] = prefixXor(valuesIn[0]) ^ prev_in_string;
  prev_in_string = static_cast<int64_t>(valuesIn[0]) >> 63;
  valuesIn[1] = prefixXor(valuesIn[1]) ^ prev_in_string;
  prev_in_string = static_cast<int64_t>(valuesIn[1]) >> 63;
  valuesIn[2] = prefixXor(valuesIn[2]) ^ prev_in_string;
  prev_in_string = static_cast<int64_t>(valuesIn[2]) >> 63;
  valuesIn[3] = prefixXor(valuesIn[3]) ^ prev_in_string;
  prev_in_string = static_cast<int64_t>(valuesIn[3]) >> 63;

  //
  // prefixXor flips on bits inside the string (and flips off the end quote).
  //
  // Then we xor with prev_in_string: if we were in a string already, its effect is flipped
  // (characters inside strings are outside, and characters outside strings are inside).
  //
  const simd::simd8<uint8_t> in_string{simd::simd8<uint8_t>::load(valuesIn)};

  // Use ^ to turn the beginning quote off, and the end quote on.

  // We are returning a function-local object so either we get a move constructor
  // or we get copy elision.
  return json_string_block(escaped, quote, in_string);
}

simdjson2_really_inline error_code json_string_scanner::finish() {
  if (prev_in_string) {
    return UNCLOSED_STRING;
  }
  return SUCCESS;
}

} // namespace stage1
} // unnamed namespace
} // namespace SIMDJSON2_IMPLEMENTATION
} // namespace simdjson2

#endif // SIMDJSON2_SRC_GENERIC_STAGE1_JSON_STRING_SCANNER_H
