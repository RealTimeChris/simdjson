#ifndef SIMDJSON2_TAPE_REF_INL_H
#define SIMDJSON2_TAPE_REF_INL_H

#include "simdjson2/dom/document.h"
#include "simdjson2/internal/tape_ref.h"
#include "simdjson2/internal/tape_type.h"

#include <cstring>

namespace simdjson2 {
namespace internal {

constexpr const uint64_t JSON_VALUE_MASK = 0x00FFFFFFFFFFFFFF;
constexpr const uint32_t JSON_COUNT_MASK = 0xFFFFFF;

//
// tape_ref inline implementation
//
simdjson2_inline tape_ref::tape_ref() noexcept : doc{nullptr}, json_index{0} {}
simdjson2_inline tape_ref::tape_ref(const dom::document *_doc, size_t _json_index) noexcept : doc{_doc}, json_index{_json_index} {}


simdjson2_inline bool tape_ref::is_document_root() const noexcept {
  return json_index == 1; // should we ever change the structure of the tape, this should get updated.
}
simdjson2_inline bool tape_ref::usable() const noexcept {
  return doc != nullptr; // when the document pointer is null, this tape_ref is uninitialized (should not be accessed).
}
// Some value types have a specific on-tape word value. It can be faster
// to check the type by doing a word-to-word comparison instead of extracting the
// most significant 8 bits.

simdjson2_inline bool tape_ref::is_double() const noexcept {
  constexpr uint64_t tape_double = uint64_t(tape_type::DOUBLE)<<56;
  return doc->tape[json_index] == tape_double;
}
simdjson2_inline bool tape_ref::is_int64() const noexcept {
  constexpr uint64_t tape_int64 = uint64_t(tape_type::INT64)<<56;
  return doc->tape[json_index] == tape_int64;
}
simdjson2_inline bool tape_ref::is_uint64() const noexcept {
  constexpr uint64_t tape_uint64 = uint64_t(tape_type::UINT64)<<56;
  return doc->tape[json_index] == tape_uint64;
}
simdjson2_inline bool tape_ref::is_false() const noexcept {
  constexpr uint64_t tape_false = uint64_t(tape_type::FALSE_VALUE)<<56;
  return doc->tape[json_index] == tape_false;
}
simdjson2_inline bool tape_ref::is_true() const noexcept {
  constexpr uint64_t tape_true = uint64_t(tape_type::TRUE_VALUE)<<56;
  return doc->tape[json_index] == tape_true;
}
simdjson2_inline bool tape_ref::is_null_on_tape() const noexcept {
  constexpr uint64_t tape_null = uint64_t(tape_type::NULL_VALUE)<<56;
  return doc->tape[json_index] == tape_null;
}

inline size_t tape_ref::after_element() const noexcept {
  switch (tape_ref_type()) {
    case tape_type::START_ARRAY:
    case tape_type::START_OBJECT:
      return matching_brace_index();
    case tape_type::UINT64:
    case tape_type::INT64:
    case tape_type::DOUBLE:
      return json_index + 2;
    default:
      return json_index + 1;
  }
}
simdjson2_inline tape_type tape_ref::tape_ref_type() const noexcept {
  return static_cast<tape_type>(doc->tape[json_index] >> 56);
}
simdjson2_inline uint64_t internal::tape_ref::tape_value() const noexcept {
  return doc->tape[json_index] & internal::JSON_VALUE_MASK;
}
simdjson2_inline uint32_t internal::tape_ref::matching_brace_index() const noexcept {
  return uint32_t(doc->tape[json_index]);
}
simdjson2_inline uint32_t internal::tape_ref::scope_count() const noexcept {
  return uint32_t((doc->tape[json_index] >> 32) & internal::JSON_COUNT_MASK);
}

template<typename T>
simdjson2_inline T tape_ref::next_tape_value() const noexcept {
  static_assert(sizeof(T) == sizeof(uint64_t), "next_tape_value() template parameter must be 64-bit");
  // Though the following is tempting...
  //  return *reinterpret_cast<const T*>(&doc->tape[json_index + 1]);
  // It is not generally safe. It is safer, and often faster to rely
  // on memcpy. Yes, it is uglier, but it is also encapsulated.
  T x;
  std::memcpy(&x,&doc->tape[json_index + 1],sizeof(uint64_t));
  return x;
}

simdjson2_inline uint32_t internal::tape_ref::get_string_length() const noexcept {
  size_t string_buf_index = size_t(tape_value());
  uint32_t len;
  std::memcpy(&len, &doc->string_buf[string_buf_index], sizeof(len));
  return len;
}

simdjson2_inline const char * internal::tape_ref::get_c_str() const noexcept {
  size_t string_buf_index = size_t(tape_value());
  return reinterpret_cast<const char *>(&doc->string_buf[string_buf_index + sizeof(uint32_t)]);
}

inline std::string_view internal::tape_ref::get_string_view() const noexcept {
  return std::string_view(
      get_c_str(),
      get_string_length()
  );
}

} // namespace internal
} // namespace simdjson2

#endif // SIMDJSON2_TAPE_REF_INL_H
