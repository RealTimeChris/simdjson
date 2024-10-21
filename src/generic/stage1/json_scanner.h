#ifndef SIMDJSON2_SRC_GENERIC_STAGE1_JSON_SCANNER_H
#include <bitset>
#ifndef SIMDJSON2_CONDITIONAL_INCLUDE
#define SIMDJSON2_SRC_GENERIC_STAGE1_JSON_SCANNER_H
#include <generic/stage1/base.h>
#include <generic/json_character_block.h>
#include <generic/stage1/json_string_scanner.h>
#endif // SIMDJSON2_CONDITIONAL_INCLUDE

namespace simdjson2 {
namespace SIMDJSON2_IMPLEMENTATION {
namespace {
namespace stage1 {

/**
 * A block of scanned json, with information on operators and scalars.
 *
 * We seek to identify pseudo-structural characters. Anything that is inside
 * a string must be omitted (hence  & ~_string.string_tail()).
 * Otherwise, pseudo-structural characters come in two forms.
 * 1. We have the structural characters ([,],{,},:, comma). The
 *    term 'structural character' is from the JSON RFC.
 * 2. We have the 'scalar pseudo-structural characters'.
 *    Scalars are quotes, and any character except structural characters and white space.
 *
 * To identify the scalar pseudo-structural characters, we must look at what comes
 * before them: it must be a space, a quote or a structural characters.
 * Starting with simdjson2 v0.3, we identify them by
 * negation: we identify everything that is followed by a non-quote scalar,
 * and we negate that. Whatever remains must be a 'scalar pseudo-structural character'.
 */
struct json_block {
public:
  // We spell out the constructors in the hope of resolving inlining issues with Visual Studio 2017
  simdjson2_inline
  json_block(json_string_block &&string, json_character_block characters,
             simd::simd8<uint8_t>& follows_potential_nonquote_scalar)
      :
  _string(std::move(string)), _characters(characters), _follows_potential_nonquote_scalar(follows_potential_nonquote_scalar) {}
  simdjson2_inline
  json_block(json_string_block string, json_character_block characters,
             simd::simd8<uint8_t>& follows_potential_nonquote_scalar)
      :
  _string(string), _characters(characters), _follows_potential_nonquote_scalar(follows_potential_nonquote_scalar) {}

  /**
   * The start of structurals.
   * In simdjson2 prior to v0.3, these were called the pseudo-structural characters.
   **/
  simdjson2_really_inline simd::simd8<uint8_t> structural_start() const noexcept {
    return potential_structural_start().bit_andnot(_string.string_tail());
  }
  /** All JSON whitespace (i.e. not in a string) */
  simdjson2_really_inline simd::simd8<uint8_t> whitespace() const noexcept { return non_quote_outside_string(_characters.whitespace()); }

  // Helpers

  /** Whether the given characters are inside a string (only works on non-quotes) */
  simdjson2_really_inline simd::simd8<uint8_t>
  non_quote_inside_string(uint64_t mask) const noexcept {
    return _string.non_quote_inside_string(mask);
  }
  /** Whether the given characters are outside a string (only works on non-quotes) */
  simdjson2_really_inline simd::simd8<uint8_t> non_quote_outside_string(const simd::simd8<uint8_t>& mask) const noexcept { return _string.non_quote_outside_string(mask); }

  // string and escape characters
  json_string_block _string;
  // whitespace, structural characters ('operators'), scalars
  json_character_block _characters;
  // whether the previous character was a scalar
  simd::simd8<uint8_t> _follows_potential_nonquote_scalar;

private:
  // Potential structurals (i.e. disregarding strings)

  /**
   * structural elements ([,],{,},:, comma) plus scalar starts like 123, true and "abc".
   * They may reside inside a string.
   **/
  simdjson2_really_inline simd::simd8<uint8_t>
  potential_structural_start() const noexcept {
    return _characters.op() | potential_scalar_start();
  }
  /**
   * The start of non-operator runs, like 123, true and "abc".
   * It main reside inside a string.
   **/
  simdjson2_really_inline simd::simd8<uint8_t> potential_scalar_start() const noexcept {
    // The term "scalar" refers to anything except structural characters and white space
    // (so letters, numbers, quotes).
    // Whenever it is preceded by something that is not a structural element ({,},[,],:, ") nor a white-space
    // then we know that it is irrelevant structurally.
    return _characters.scalar().bit_andnot(follows_potential_scalar());
  }
  /**
   * Whether the given character is immediately after a non-operator like 123, true.
   * The characters following a quote are not included.
   */
  simdjson2_really_inline simd::simd8<uint8_t> follows_potential_scalar() const noexcept {
    // _follows_potential_nonquote_scalar: is defined as marking any character that follows a character
    // that is not a structural element ({,},[,],:, comma) nor a quote (") and that is not a
    // white space.
    // It is understood that within quoted region, anything at all could be marked (irrelevant).
    return _follows_potential_nonquote_scalar;
  }
};

/**
 * Scans JSON for important bits: structural characters or 'operators', strings, and scalars.
 *
 * The scanner starts by calculating two distinct things:
 * - string characters (taking \" into account)
 * - structural characters or 'operators' ([]{},:, comma)
 *   and scalars (runs of non-operators like 123, true and "abc")
 *
 * To minimize data dependency (a key component of the scanner's speed), it finds these in parallel:
 * in particular, the operator/scalar bit will find plenty of things that are actually part of
 * strings. When we're done, json_block will fuse the two together by masking out tokens that are
 * part of a string.
 */
class json_scanner {
public:
  json_scanner() = default;
  simdjson2_really_inline json_block next(const simd::simd8<uint8_t> (&in)[8]);
  // Returns either UNCLOSED_STRING or SUCCESS
  simdjson2_really_inline error_code finish();

private:
  // Whether the last character of the previous iteration is part of a scalar token
  // (anything except whitespace or a structural character/'operator').
  bool prev_scalar = 0ULL;
  json_string_scanner string_scanner{};
};


//
// Check if the current character immediately follows a matching character.
//
// For example, this checks for quotes with backslashes in front of them:
//
//     const uint64_t backslashed_quote = in.eq('"') & immediately_follows(in.eq('\'), prev_backslash);
//

alignas(32) uint64_t values[4 * 2];
simdjson2_really_inline simd::simd8<uint8_t> follows(const simd::simd8<uint8_t>& match,
                                                bool &overflow) {
  bool oldOverflow = overflow;
  overflow = match.get_msb();
  simd::simd8<uint8_t> result;
  match.store(values);
  static constexpr uint64_t shiftAmount{64 - 1};
  values[4] = values[0] << 1;
  values[1 + 4] = values[1] << 1 | values[1 - 1] >> (shiftAmount);
  values[2 + 4] = values[2] << 1 | values[2 - 1] >> (shiftAmount);
  values[3 + 4] = values[3] << 1 | values[3 - 1] >> (shiftAmount);
  result = simd::simd8<uint8_t>::load(values + 4);
  result.set_lsb(oldOverflow);
  return result;
}

simdjson2_really_inline json_block json_scanner::next(const simd::simd8<uint8_t>(&in)[8]) {
  json_string_block strings = string_scanner.next(in);
  // identifies the white-space and the structural characters
  json_character_block characters = json_character_block::classify(in);
  // The term "scalar" refers to anything except structural characters and white space
  // (so letters, numbers, quotes).
  // We want follows_scalar to mark anything that follows a non-quote scalar (so letters and numbers).
  //
  // A terminal quote should either be followed by a structural character (comma, brace, bracket, colon)
  // or nothing. However, we still want ' "a string"true ' to mark the 't' of 'true' as a potential
  // pseudo-structural character just like we would if we had  ' "a string" true '; otherwise we
  // may need to add an extra check when parsing strings.
  //
  // Performance: there are many ways to skin this cat.
  const simd::simd8<uint8_t> nonquote_scalar =
      characters.scalar().bit_andnot(strings.quote());
  simd::simd8<uint8_t> follows_nonquote_scalar =
      follows(nonquote_scalar, prev_scalar);
  // We are returning a function-local object so either we get a move constructor
  // or we get copy elision.
  return json_block(
    strings,// strings is a function-local object so either it moves or the copy is elided.
    characters,
    follows_nonquote_scalar
  );
}

simdjson2_really_inline error_code json_scanner::finish() {
  return string_scanner.finish();
}

} // namespace stage1
} // unnamed namespace
} // namespace SIMDJSON2_IMPLEMENTATION
} // namespace simdjson2

#endif // SIMDJSON2_SRC_GENERIC_STAGE1_JSON_SCANNER_H
