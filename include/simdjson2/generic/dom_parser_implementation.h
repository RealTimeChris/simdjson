#ifndef SIMDJSON2_GENERIC_DOM_PARSER_IMPLEMENTATION_H

#ifndef SIMDJSON2_CONDITIONAL_INCLUDE
#define SIMDJSON2_GENERIC_DOM_PARSER_IMPLEMENTATION_H
#include "simdjson2/generic/base.h"
#include "simdjson2/internal/dom_parser_implementation.h"
#endif // SIMDJSON2_CONDITIONAL_INCLUDE

namespace simdjson2 {
namespace SIMDJSON2_IMPLEMENTATION {

// expectation: sizeof(open_container) = 64/8.
struct open_container {
  uint32_t tape_index; // where, on the tape, does the scope ([,{) begins
  uint32_t count; // how many elements in the scope
}; // struct open_container

static_assert(sizeof(open_container) == 64/8, "Open container must be 64 bits");

class dom_parser_implementation final : public internal::dom_parser_implementation {
public:
  /** Tape location of each open { or [ */
  std::unique_ptr<open_container[]> open_containers{};
  /** Whether each open container is a [ or { */
  std::unique_ptr<bool[]> is_array{};
  /** Buffer passed to stage 1 */
  const uint8_t *buf{};
  /** Length passed to stage 1 */
  size_t len{0};
  /** Document passed to stage 2 */
  dom::document *doc{};

  inline dom_parser_implementation() noexcept;
  inline dom_parser_implementation(dom_parser_implementation &&other) noexcept;
  inline dom_parser_implementation &operator=(dom_parser_implementation &&other) noexcept;
  dom_parser_implementation(const dom_parser_implementation &) = delete;
  dom_parser_implementation &operator=(const dom_parser_implementation &) = delete;

  simdjson2_warn_unused error_code parse(const uint8_t *buf, size_t len, dom::document &doc) noexcept final;
  simdjson2_warn_unused error_code stage1(const uint8_t *buf, size_t len, stage1_mode partial) noexcept final;
  simdjson2_warn_unused error_code stage2(dom::document &doc) noexcept final;
  simdjson2_warn_unused error_code stage2_next(dom::document &doc) noexcept final;
  simdjson2_warn_unused uint8_t *parse_string(const uint8_t *src, uint8_t *dst, bool allow_replacement) const noexcept final;
  simdjson2_warn_unused uint8_t *parse_wobbly_string(const uint8_t *src, uint8_t *dst) const noexcept final;
  inline simdjson2_warn_unused error_code set_capacity(size_t capacity) noexcept final;
  inline simdjson2_warn_unused error_code set_max_depth(size_t max_depth) noexcept final;
private:
  simdjson2_really_inline simdjson2_warn_unused error_code set_capacity_stage1(size_t capacity);

};

} // namespace SIMDJSON2_IMPLEMENTATION
} // namespace simdjson2

namespace simdjson2 {
namespace SIMDJSON2_IMPLEMENTATION {

inline dom_parser_implementation::dom_parser_implementation() noexcept = default;
inline dom_parser_implementation::dom_parser_implementation(dom_parser_implementation &&other) noexcept = default;
inline dom_parser_implementation &dom_parser_implementation::operator=(dom_parser_implementation &&other) noexcept = default;

// Leaving these here so they can be inlined if so desired
inline simdjson2_warn_unused error_code dom_parser_implementation::set_capacity(size_t capacity) noexcept {
  if(capacity > SIMDJSON2_MAXSIZE_BYTES) { return CAPACITY; }
  // Stage 1 index output
  size_t max_structures = SIMDJSON2_ROUNDUP_N(capacity, 64) + 2 + 7;
  structural_indexes.reset( new (std::nothrow) uint32_t[max_structures] );
  if (!structural_indexes) { _capacity = 0; return MEMALLOC; }
  structural_indexes[0] = 0;
  n_structural_indexes = 0;

  _capacity = capacity;
  return SUCCESS;
}

inline simdjson2_warn_unused error_code dom_parser_implementation::set_max_depth(size_t max_depth) noexcept {
  // Stage 2 stacks
  open_containers.reset(new (std::nothrow) open_container[max_depth]);
  is_array.reset(new (std::nothrow) bool[max_depth]);
  if (!is_array || !open_containers) { _max_depth = 0; return MEMALLOC; }

  _max_depth = max_depth;
  return SUCCESS;
}

} // namespace SIMDJSON2_IMPLEMENTATION
} // namespace simdjson2

#endif // SIMDJSON2_GENERIC_DOM_PARSER_IMPLEMENTATION_H
