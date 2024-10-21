#ifndef SIMDJSON2_SRC_HASWELL_CPP
#define SIMDJSON2_SRC_HASWELL_CPP

#ifndef SIMDJSON2_CONDITIONAL_INCLUDE
#include <base.h>
#endif // SIMDJSON2_CONDITIONAL_INCLUDE

#include <simdjson2/haswell.h>
#include <simdjson2/haswell/implementation.h>

#include <simdjson2/haswell/begin.h>
#include <generic/amalgamated.h>
#include <generic/stage1/amalgamated.h>
#include <generic/stage2/amalgamated.h>

//
// Stage 1
//

namespace simdjson2 {
namespace SIMDJSON2_IMPLEMENTATION {

simdjson2_warn_unused error_code implementation::create_dom_parser_implementation(
  size_t capacity,
  size_t max_depth,
  std::unique_ptr<internal::dom_parser_implementation>& dst
) const noexcept {
  dst.reset( new (std::nothrow) dom_parser_implementation() );
  if (!dst) { return MEMALLOC; }
  if (auto err = dst->set_capacity(capacity))
    return err;
  if (auto err = dst->set_max_depth(max_depth))
    return err;
  return SUCCESS;
}

namespace {

using namespace simd;

// This identifies structural characters (comma, colon, braces, brackets),
// and ASCII white-space ('\r','\n','\t',' ').
simdjson2_really_inline json_character_block
json_character_block::classify(const simd::simd8<uint8_t> (&in)[8]) {
  // These lookups rely on the fact that anything < 127 will match the lower 4 bits, which is why
  // we can't use the generic lookup_16.
  const auto whitespace_table = simd8<uint8_t>::repeat_16(' ', 100, 100, 100, 17, 100, 113, 2, 100, '\t', '\n', 112, 100, '\r', 100, 100);

  // The 6 operators (:,[]{}) have these values:
  //
  // , 2C
  // : 3A
  // [ 5B
  // { 7B
  // ] 5D
  // } 7D
  //
  // If you use | 0x20 to turn [ and ] into { and }, the lower 4 bits of each character is unique.
  // We exploit this, using a simd 4-bit lookup to tell us which character match against, and then
  // match it (against | 0x20).
  //
  // To prevent recognizing other characters, everything else gets compared with 0, which cannot
  // match due to the | 0x20.
  //
  // NOTE: Due to the | 0x20, this ALSO treats <FF> and <SUB> (control characters 0C and 1A) like ,
  // and :. This gets caught in stage 2, which checks the actual character to ensure the right
  // operators are in the right places.
  const auto op_table = simd8<uint8_t>::repeat_16(
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, ':', '{', // : = 3A, [ = 5B, { = 7B
    ',', '}', 0, 0  // , = 2C, ] = 5D, } = 7D
  );

  // We compute whitespace and op separately. If later code only uses one or the
  // other, given the fact that all functions are aggressively inlined, we can
  // hope that useless computations will be omitted. This is namely case when
  // minifying (we only need whitespace).

  alignas(32) uint32_t whitespace_pre[8];
  for (size_t x = 0; x < 8; ++x) {
    whitespace_pre[x] = _mm256_movemask_epi8(
        in[x].eq({_mm256_shuffle_epi8(whitespace_table, in[x])}));
  }
  const simd8<uint8_t> whitespace{simd::simd8<uint8_t>::load(whitespace_pre)};

  alignas(32) uint32_t op_pre[8];
  for (size_t x = 0; x < 8; ++x) {
    const simd8<uint8_t> curlified{in[x] | 0x20};
    op_pre[x] = _mm256_movemask_epi8(
        curlified.eq({_mm256_shuffle_epi8(op_table, in[x])}));
  }
  const simd8<uint8_t> op{simd::simd8<uint8_t>::load(op_pre)};
  return { whitespace, op };
}

simdjson2_really_inline bool is_ascii(const simd8x64<uint8_t>& input) {
  return input.reduce_or().is_ascii();
}

simdjson2_unused simdjson2_really_inline simd8<bool> must_be_continuation(const simd8<uint8_t> prev1, const simd8<uint8_t> prev2, const simd8<uint8_t> prev3) {
  simd8<uint8_t> is_second_byte = prev1.saturating_sub(0xc0u-1); // Only 11______ will be > 0
  simd8<uint8_t> is_third_byte  = prev2.saturating_sub(0xe0u-1); // Only 111_____ will be > 0
  simd8<uint8_t> is_fourth_byte = prev3.saturating_sub(0xf0u-1); // Only 1111____ will be > 0
  // Caller requires a bool (all 1's). All values resulting from the subtraction will be <= 64, so signed comparison is fine.
  return simd8<int8_t>(is_second_byte | is_third_byte | is_fourth_byte) > int8_t(0);
}

simdjson2_really_inline simd8<uint8_t> must_be_2_3_continuation(const simd8<uint8_t> prev2, const simd8<uint8_t> prev3) {
  simd8<uint8_t> is_third_byte  = prev2.saturating_sub(0xe0u-0x80); // Only 111_____ will be >= 0x80
  simd8<uint8_t> is_fourth_byte = prev3.saturating_sub(0xf0u-0x80); // Only 1111____ will be >= 0x80
  return is_third_byte | is_fourth_byte;
}

} // unnamed namespace
} // namespace SIMDJSON2_IMPLEMENTATION
} // namespace simdjson2

//
// Stage 2
//

//
// Implementation-specific overrides
//
namespace simdjson2 {
namespace SIMDJSON2_IMPLEMENTATION {

simdjson2_warn_unused error_code implementation::minify(const uint8_t *buf, size_t len, uint8_t *dst, size_t &dst_len) const noexcept {
  return haswell::stage1::json_minifier::minify<256>(buf, len, dst, dst_len);
}

simdjson2_warn_unused error_code dom_parser_implementation::stage1(const uint8_t *_buf, size_t _len, stage1_mode streaming) noexcept {
  this->buf = _buf;
  this->len = _len;
  haswell::stage1::json_structural_indexer indexer{};
  return indexer.index<256>(_buf, _len, *this, streaming);
}

simdjson2_warn_unused bool implementation::validate_utf8(const char *buf, size_t len) const noexcept {
  return haswell::stage1::generic_validate_utf8(buf,len);
}

simdjson2_warn_unused error_code dom_parser_implementation::stage2(dom::document &_doc) noexcept {
  return stage2::tape_builder::parse_document<false>(*this, _doc);
}

simdjson2_warn_unused error_code dom_parser_implementation::stage2_next(dom::document &_doc) noexcept {
  return stage2::tape_builder::parse_document<true>(*this, _doc);
}

simdjson2_warn_unused uint8_t *dom_parser_implementation::parse_string(const uint8_t *src, uint8_t *dst, bool replacement_char) const noexcept {
  return haswell::stringparsing::parse_string(src, dst, replacement_char);
}

simdjson2_warn_unused uint8_t *dom_parser_implementation::parse_wobbly_string(const uint8_t *src, uint8_t *dst) const noexcept {
  return haswell::stringparsing::parse_wobbly_string(src, dst);
}

simdjson2_warn_unused error_code dom_parser_implementation::parse(const uint8_t *_buf, size_t _len, dom::document &_doc) noexcept {
  auto error = stage1(_buf, _len, stage1_mode::regular);
  if (error) { return error; }
  return stage2(_doc);
}

} // namespace SIMDJSON2_IMPLEMENTATION
} // namespace simdjson2

#include <simdjson2/haswell/end.h>

#endif // SIMDJSON2_SRC_HASWELL_CPP
