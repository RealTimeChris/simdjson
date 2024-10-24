#ifndef SIMDJSON2_SRC_GENERIC_STAGE2_STRUCTURAL_ITERATOR_H

#ifndef SIMDJSON2_CONDITIONAL_INCLUDE
#define SIMDJSON2_SRC_GENERIC_STAGE2_STRUCTURAL_ITERATOR_H
#include <generic/stage2/base.h>
#include <simdjson2/generic/dom_parser_implementation.h>
#endif // SIMDJSON2_CONDITIONAL_INCLUDE

namespace simdjson2 {
namespace SIMDJSON2_IMPLEMENTATION {
namespace {
namespace stage2 {

class structural_iterator {
public:
  const uint8_t* const buf;
  uint32_t *next_structural;
  dom_parser_implementation &dom_parser;

  // Start a structural
  simdjson2_inline structural_iterator(dom_parser_implementation &_dom_parser, size_t start_structural_index)
    : buf{_dom_parser.buf},
      next_structural{&_dom_parser.structural_indexes[start_structural_index]},
      dom_parser{_dom_parser} {
  }
  // Get the buffer position of the current structural character
  simdjson2_inline const uint8_t* current() {
    return &buf[*(next_structural-1)];
  }
  // Get the current structural character
  simdjson2_inline char current_char() {
    return buf[*(next_structural-1)];
  }
  // Get the next structural character without advancing
  simdjson2_inline char peek_next_char() {
    return buf[*next_structural];
  }
  simdjson2_inline const uint8_t* peek() {
    return &buf[*next_structural];
  }
  simdjson2_inline const uint8_t* advance() {
    return &buf[*(next_structural++)];
  }
  simdjson2_inline char advance_char() {
    return buf[*(next_structural++)];
  }
  simdjson2_inline size_t remaining_len() {
    return dom_parser.len - *(next_structural-1);
  }

  simdjson2_inline bool at_end() {
    return next_structural == &dom_parser.structural_indexes[dom_parser.n_structural_indexes];
  }
  simdjson2_inline bool at_beginning() {
    return next_structural == dom_parser.structural_indexes.get();
  }
};

} // namespace stage2
} // unnamed namespace
} // namespace SIMDJSON2_IMPLEMENTATION
} // namespace simdjson2

#endif // SIMDJSON2_SRC_GENERIC_STAGE2_STRUCTURAL_ITERATOR_H