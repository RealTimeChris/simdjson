#ifndef SIMDJSON2_INTERNAL_TAPE_REF_H
#define SIMDJSON2_INTERNAL_TAPE_REF_H

#include "simdjson2/base.h"

namespace simdjson2 {
namespace dom {
class document;
} // namespace dom

namespace internal {

/**
 * A reference to an element on the tape. Internal only.
 */
class tape_ref {
public:
  simdjson2_inline tape_ref() noexcept;
  simdjson2_inline tape_ref(const dom::document *doc, size_t json_index) noexcept;
  inline size_t after_element() const noexcept;
  simdjson2_inline tape_type tape_ref_type() const noexcept;
  simdjson2_inline uint64_t tape_value() const noexcept;
  simdjson2_inline bool is_double() const noexcept;
  simdjson2_inline bool is_int64() const noexcept;
  simdjson2_inline bool is_uint64() const noexcept;
  simdjson2_inline bool is_false() const noexcept;
  simdjson2_inline bool is_true() const noexcept;
  simdjson2_inline bool is_null_on_tape() const noexcept;// different name to avoid clash with is_null.
  simdjson2_inline uint32_t matching_brace_index() const noexcept;
  simdjson2_inline uint32_t scope_count() const noexcept;
  template<typename T>
  simdjson2_inline T next_tape_value() const noexcept;
  simdjson2_inline uint32_t get_string_length() const noexcept;
  simdjson2_inline const char * get_c_str() const noexcept;
  inline std::string_view get_string_view() const noexcept;
  simdjson2_inline bool is_document_root() const noexcept;
  simdjson2_inline bool usable() const noexcept;

  /** The document this element references. */
  const dom::document *doc;

  /** The index of this element on `doc.tape[]` */
  size_t json_index;
};

} // namespace internal
} // namespace simdjson2

#endif // SIMDJSON2_INTERNAL_TAPE_REF_H
