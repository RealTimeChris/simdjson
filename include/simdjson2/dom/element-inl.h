#ifndef SIMDJSON2_ELEMENT_INL_H
#define SIMDJSON2_ELEMENT_INL_H

#include "simdjson2/dom/base.h"
#include "simdjson2/dom/element.h"
#include "simdjson2/dom/document.h"
#include "simdjson2/dom/object.h"
#include "simdjson2/internal/tape_type.h"

#include "simdjson2/dom/object-inl.h"
#include "simdjson2/error-inl.h"
#include "simdjson2/jsonpathutil.h"

#include <ostream>
#include <limits>

namespace simdjson2 {

//
// simdjson2_result<dom::element> inline implementation
//
simdjson2_inline simdjson2_result<dom::element>::simdjson2_result() noexcept
    : internal::simdjson2_result_base<dom::element>() {}
simdjson2_inline simdjson2_result<dom::element>::simdjson2_result(dom::element &&value) noexcept
    : internal::simdjson2_result_base<dom::element>(std::forward<dom::element>(value)) {}
simdjson2_inline simdjson2_result<dom::element>::simdjson2_result(error_code error) noexcept
    : internal::simdjson2_result_base<dom::element>(error) {}
inline simdjson2_result<dom::element_type> simdjson2_result<dom::element>::type() const noexcept {
  if (error()) { return error(); }
  return first.type();
}

template<typename T>
simdjson2_inline bool simdjson2_result<dom::element>::is() const noexcept {
  return !error() && first.is<T>();
}
template<typename T>
simdjson2_inline simdjson2_result<T> simdjson2_result<dom::element>::get() const noexcept {
  if (error()) { return error(); }
  return first.get<T>();
}
template<typename T>
simdjson2_warn_unused simdjson2_inline error_code simdjson2_result<dom::element>::get(T &value) const noexcept {
  if (error()) { return error(); }
  return first.get<T>(value);
}

simdjson2_inline simdjson2_result<dom::array> simdjson2_result<dom::element>::get_array() const noexcept {
  if (error()) { return error(); }
  return first.get_array();
}
simdjson2_inline simdjson2_result<dom::object> simdjson2_result<dom::element>::get_object() const noexcept {
  if (error()) { return error(); }
  return first.get_object();
}
simdjson2_inline simdjson2_result<const char *> simdjson2_result<dom::element>::get_c_str() const noexcept {
  if (error()) { return error(); }
  return first.get_c_str();
}
simdjson2_inline simdjson2_result<size_t> simdjson2_result<dom::element>::get_string_length() const noexcept {
  if (error()) { return error(); }
  return first.get_string_length();
}
simdjson2_inline simdjson2_result<std::string_view> simdjson2_result<dom::element>::get_string() const noexcept {
  if (error()) { return error(); }
  return first.get_string();
}
simdjson2_inline simdjson2_result<int64_t> simdjson2_result<dom::element>::get_int64() const noexcept {
  if (error()) { return error(); }
  return first.get_int64();
}
simdjson2_inline simdjson2_result<uint64_t> simdjson2_result<dom::element>::get_uint64() const noexcept {
  if (error()) { return error(); }
  return first.get_uint64();
}
simdjson2_inline simdjson2_result<double> simdjson2_result<dom::element>::get_double() const noexcept {
  if (error()) { return error(); }
  return first.get_double();
}
simdjson2_inline simdjson2_result<bool> simdjson2_result<dom::element>::get_bool() const noexcept {
  if (error()) { return error(); }
  return first.get_bool();
}

simdjson2_inline bool simdjson2_result<dom::element>::is_array() const noexcept {
  return !error() && first.is_array();
}
simdjson2_inline bool simdjson2_result<dom::element>::is_object() const noexcept {
  return !error() && first.is_object();
}
simdjson2_inline bool simdjson2_result<dom::element>::is_string() const noexcept {
  return !error() && first.is_string();
}
simdjson2_inline bool simdjson2_result<dom::element>::is_int64() const noexcept {
  return !error() && first.is_int64();
}
simdjson2_inline bool simdjson2_result<dom::element>::is_uint64() const noexcept {
  return !error() && first.is_uint64();
}
simdjson2_inline bool simdjson2_result<dom::element>::is_double() const noexcept {
  return !error() && first.is_double();
}
simdjson2_inline bool simdjson2_result<dom::element>::is_number() const noexcept {
  return !error() && first.is_number();
}
simdjson2_inline bool simdjson2_result<dom::element>::is_bool() const noexcept {
  return !error() && first.is_bool();
}

simdjson2_inline bool simdjson2_result<dom::element>::is_null() const noexcept {
  return !error() && first.is_null();
}

simdjson2_inline simdjson2_result<dom::element> simdjson2_result<dom::element>::operator[](std::string_view key) const noexcept {
  if (error()) { return error(); }
  return first[key];
}
simdjson2_inline simdjson2_result<dom::element> simdjson2_result<dom::element>::operator[](const char *key) const noexcept {
  if (error()) { return error(); }
  return first[key];
}
simdjson2_inline simdjson2_result<dom::element> simdjson2_result<dom::element>::at_pointer(const std::string_view json_pointer) const noexcept {
  if (error()) { return error(); }
  return first.at_pointer(json_pointer);
}
simdjson2_inline simdjson2_result<dom::element> simdjson2_result<dom::element>::at_path(const std::string_view json_path) const noexcept {
  auto json_pointer = json_path_to_pointer_conversion(json_path);
  if (json_pointer == "-1") { return INVALID_JSON_POINTER; }
  return at_pointer(json_pointer);
}
#ifndef SIMDJSON2_DISABLE_DEPRECATED_API
[[deprecated("For standard compliance, use at_pointer instead, and prefix your pointers with a slash '/', see RFC6901 ")]]
simdjson2_inline simdjson2_result<dom::element> simdjson2_result<dom::element>::at(const std::string_view json_pointer) const noexcept {
SIMDJSON2_PUSH_DISABLE_WARNINGS
SIMDJSON2_DISABLE_DEPRECATED_WARNING
  if (error()) { return error(); }
  return first.at(json_pointer);
SIMDJSON2_POP_DISABLE_WARNINGS
}
#endif // SIMDJSON2_DISABLE_DEPRECATED_API
simdjson2_inline simdjson2_result<dom::element> simdjson2_result<dom::element>::at(size_t index) const noexcept {
  if (error()) { return error(); }
  return first.at(index);
}
simdjson2_inline simdjson2_result<dom::element> simdjson2_result<dom::element>::at_key(std::string_view key) const noexcept {
  if (error()) { return error(); }
  return first.at_key(key);
}
simdjson2_inline simdjson2_result<dom::element> simdjson2_result<dom::element>::at_key_case_insensitive(std::string_view key) const noexcept {
  if (error()) { return error(); }
  return first.at_key_case_insensitive(key);
}

#if SIMDJSON2_EXCEPTIONS

simdjson2_inline simdjson2_result<dom::element>::operator bool() const noexcept(false) {
  return get<bool>();
}
simdjson2_inline simdjson2_result<dom::element>::operator const char *() const noexcept(false) {
  return get<const char *>();
}
simdjson2_inline simdjson2_result<dom::element>::operator std::string_view() const noexcept(false) {
  return get<std::string_view>();
}
simdjson2_inline simdjson2_result<dom::element>::operator uint64_t() const noexcept(false) {
  return get<uint64_t>();
}
simdjson2_inline simdjson2_result<dom::element>::operator int64_t() const noexcept(false) {
  return get<int64_t>();
}
simdjson2_inline simdjson2_result<dom::element>::operator double() const noexcept(false) {
  return get<double>();
}
simdjson2_inline simdjson2_result<dom::element>::operator dom::array() const noexcept(false) {
  return get<dom::array>();
}
simdjson2_inline simdjson2_result<dom::element>::operator dom::object() const noexcept(false) {
  return get<dom::object>();
}

simdjson2_inline dom::array::iterator simdjson2_result<dom::element>::begin() const noexcept(false) {
  if (error()) { throw simdjson2_error(error()); }
  return first.begin();
}
simdjson2_inline dom::array::iterator simdjson2_result<dom::element>::end() const noexcept(false) {
  if (error()) { throw simdjson2_error(error()); }
  return first.end();
}

#endif // SIMDJSON2_EXCEPTIONS

namespace dom {

//
// element inline implementation
//
simdjson2_inline element::element() noexcept : tape{} {}
simdjson2_inline element::element(const internal::tape_ref &_tape) noexcept : tape{_tape} { }

inline element_type element::type() const noexcept {
  SIMDJSON2_DEVELOPMENT_ASSERT(tape.usable()); // https://github.com/simdjson2/simdjson2/issues/1914
  auto tape_type = tape.tape_ref_type();
  return tape_type == internal::tape_type::FALSE_VALUE ? element_type::BOOL : static_cast<element_type>(tape_type);
}

inline simdjson2_result<bool> element::get_bool() const noexcept {
  SIMDJSON2_DEVELOPMENT_ASSERT(tape.usable()); // https://github.com/simdjson2/simdjson2/issues/1914
  if(tape.is_true()) {
    return true;
  } else if(tape.is_false()) {
    return false;
  }
  return INCORRECT_TYPE;
}
inline simdjson2_result<const char *> element::get_c_str() const noexcept {
  SIMDJSON2_DEVELOPMENT_ASSERT(tape.usable()); // https://github.com/simdjson2/simdjson2/issues/1914
  switch (tape.tape_ref_type()) {
    case internal::tape_type::STRING: {
      return tape.get_c_str();
    }
    default:
      return INCORRECT_TYPE;
  }
}
inline simdjson2_result<size_t> element::get_string_length() const noexcept {
  SIMDJSON2_DEVELOPMENT_ASSERT(tape.usable()); // https://github.com/simdjson2/simdjson2/issues/1914
  switch (tape.tape_ref_type()) {
    case internal::tape_type::STRING: {
      return tape.get_string_length();
    }
    default:
      return INCORRECT_TYPE;
  }
}
inline simdjson2_result<std::string_view> element::get_string() const noexcept {
  SIMDJSON2_DEVELOPMENT_ASSERT(tape.usable()); // https://github.com/simdjson2/simdjson2/issues/1914
  switch (tape.tape_ref_type()) {
    case internal::tape_type::STRING:
      return tape.get_string_view();
    default:
      return INCORRECT_TYPE;
  }
}
inline simdjson2_result<uint64_t> element::get_uint64() const noexcept {
  SIMDJSON2_DEVELOPMENT_ASSERT(tape.usable()); // https://github.com/simdjson2/simdjson2/issues/1914
  if(simdjson2_unlikely(!tape.is_uint64())) { // branch rarely taken
    if(tape.is_int64()) {
      int64_t result = tape.next_tape_value<int64_t>();
      if (result < 0) {
        return NUMBER_OUT_OF_RANGE;
      }
      return uint64_t(result);
    }
    return INCORRECT_TYPE;
  }
  return tape.next_tape_value<int64_t>();
}
inline simdjson2_result<int64_t> element::get_int64() const noexcept {
  SIMDJSON2_DEVELOPMENT_ASSERT(tape.usable()); // https://github.com/simdjson2/simdjson2/issues/1914
  if(simdjson2_unlikely(!tape.is_int64())) { // branch rarely taken
    if(tape.is_uint64()) {
      uint64_t result = tape.next_tape_value<uint64_t>();
      // Wrapping max in parens to handle Windows issue: https://stackoverflow.com/questions/11544073/how-do-i-deal-with-the-max-macro-in-windows-h-colliding-with-max-in-std
      if (result > uint64_t((std::numeric_limits<int64_t>::max)())) {
        return NUMBER_OUT_OF_RANGE;
      }
      return static_cast<int64_t>(result);
    }
    return INCORRECT_TYPE;
  }
  return tape.next_tape_value<int64_t>();
}
inline simdjson2_result<double> element::get_double() const noexcept {
  SIMDJSON2_DEVELOPMENT_ASSERT(tape.usable()); // https://github.com/simdjson2/simdjson2/issues/1914
  // Performance considerations:
  // 1. Querying tape_ref_type() implies doing a shift, it is fast to just do a straight
  //   comparison.
  // 2. Using a switch-case relies on the compiler guessing what kind of code generation
  //    we want... But the compiler cannot know that we expect the type to be "double"
  //    most of the time.
  // We can expect get<double> to refer to a double type almost all the time.
  // It is important to craft the code accordingly so that the compiler can use this
  // information. (This could also be solved with profile-guided optimization.)
  if(simdjson2_unlikely(!tape.is_double())) { // branch rarely taken
    if(tape.is_uint64()) {
      return double(tape.next_tape_value<uint64_t>());
    } else if(tape.is_int64()) {
      return double(tape.next_tape_value<int64_t>());
    }
    return INCORRECT_TYPE;
  }
  // this is common:
  return tape.next_tape_value<double>();
}
inline simdjson2_result<array> element::get_array() const noexcept {
  SIMDJSON2_DEVELOPMENT_ASSERT(tape.usable()); // https://github.com/simdjson2/simdjson2/issues/1914
  switch (tape.tape_ref_type()) {
    case internal::tape_type::START_ARRAY:
      return array(tape);
    default:
      return INCORRECT_TYPE;
  }
}
inline simdjson2_result<object> element::get_object() const noexcept {
  SIMDJSON2_DEVELOPMENT_ASSERT(tape.usable()); // https://github.com/simdjson2/simdjson2/issues/1914
  switch (tape.tape_ref_type()) {
    case internal::tape_type::START_OBJECT:
      return object(tape);
    default:
      return INCORRECT_TYPE;
  }
}

template<typename T>
simdjson2_warn_unused simdjson2_inline error_code element::get(T &value) const noexcept {
  return get<T>().get(value);
}
// An element-specific version prevents recursion with simdjson2_result::get<element>(value)
template<>
simdjson2_warn_unused simdjson2_inline error_code element::get<element>(element &value) const noexcept {
  value = element(tape);
  return SUCCESS;
}
template<typename T>
inline void element::tie(T &value, error_code &error) && noexcept {
  error = get<T>(value);
}

template<typename T>
simdjson2_inline bool element::is() const noexcept {
  auto result = get<T>();
  return !result.error();
}

template<> inline simdjson2_result<array> element::get<array>() const noexcept { return get_array(); }
template<> inline simdjson2_result<object> element::get<object>() const noexcept { return get_object(); }
template<> inline simdjson2_result<const char *> element::get<const char *>() const noexcept { return get_c_str(); }
template<> inline simdjson2_result<std::string_view> element::get<std::string_view>() const noexcept { return get_string(); }
template<> inline simdjson2_result<int64_t> element::get<int64_t>() const noexcept { return get_int64(); }
template<> inline simdjson2_result<uint64_t> element::get<uint64_t>() const noexcept { return get_uint64(); }
template<> inline simdjson2_result<double> element::get<double>() const noexcept { return get_double(); }
template<> inline simdjson2_result<bool> element::get<bool>() const noexcept { return get_bool(); }

inline bool element::is_array() const noexcept { return is<array>(); }
inline bool element::is_object() const noexcept { return is<object>(); }
inline bool element::is_string() const noexcept { return is<std::string_view>(); }
inline bool element::is_int64() const noexcept { return is<int64_t>(); }
inline bool element::is_uint64() const noexcept { return is<uint64_t>(); }
inline bool element::is_double() const noexcept { return is<double>(); }
inline bool element::is_bool() const noexcept { return is<bool>(); }
inline bool element::is_number() const noexcept { return is_int64() || is_uint64() || is_double(); }

inline bool element::is_null() const noexcept {
  return tape.is_null_on_tape();
}

#if SIMDJSON2_EXCEPTIONS

inline element::operator bool() const noexcept(false) { return get<bool>(); }
inline element::operator const char*() const noexcept(false) { return get<const char *>(); }
inline element::operator std::string_view() const noexcept(false) { return get<std::string_view>(); }
inline element::operator uint64_t() const noexcept(false) { return get<uint64_t>(); }
inline element::operator int64_t() const noexcept(false) { return get<int64_t>(); }
inline element::operator double() const noexcept(false) { return get<double>(); }
inline element::operator array() const noexcept(false) { return get<array>(); }
inline element::operator object() const noexcept(false) { return get<object>(); }

inline array::iterator element::begin() const noexcept(false) {
  return get<array>().begin();
}
inline array::iterator element::end() const noexcept(false) {
  return get<array>().end();
}

#endif // SIMDJSON2_EXCEPTIONS

inline simdjson2_result<element> element::operator[](std::string_view key) const noexcept {
  return at_key(key);
}
inline simdjson2_result<element> element::operator[](const char *key) const noexcept {
  return at_key(key);
}

inline bool is_pointer_well_formed(std::string_view json_pointer) noexcept {
  if (simdjson2_unlikely(json_pointer[0] != '/')) {
    return false;
  }
  size_t escape = json_pointer.find('~');
  if (escape == std::string_view::npos) {
    return true;
  }
  if (escape == json_pointer.size() - 1) {
    return false;
  }
  if (json_pointer[escape + 1] != '0' && json_pointer[escape + 1] != '1') {
    return false;
  }
  return true;
}

inline simdjson2_result<element> element::at_pointer(std::string_view json_pointer) const noexcept {
  SIMDJSON2_DEVELOPMENT_ASSERT(tape.usable()); // https://github.com/simdjson2/simdjson2/issues/1914
  switch (tape.tape_ref_type()) {
    case internal::tape_type::START_OBJECT:
      return object(tape).at_pointer(json_pointer);
    case internal::tape_type::START_ARRAY:
      return array(tape).at_pointer(json_pointer);
    default: {
      if (!json_pointer.empty()) { // a non-empty string can be invalid, or accessing a primitive (issue 2154)
        if (is_pointer_well_formed(json_pointer)) {
          return NO_SUCH_FIELD;
        }
        return INVALID_JSON_POINTER;
      }
      // an empty string means that we return the current node
      dom::element copy(*this);
      return simdjson2_result<element>(std::move(copy));
    }
  }
}
inline simdjson2_result<element> element::at_path(std::string_view json_path) const noexcept {
  auto json_pointer = json_path_to_pointer_conversion(json_path);
  if (json_pointer == "-1") { return INVALID_JSON_POINTER; }
  return at_pointer(json_pointer);
}
#ifndef SIMDJSON2_DISABLE_DEPRECATED_API
[[deprecated("For standard compliance, use at_pointer instead, and prefix your pointers with a slash '/', see RFC6901 ")]]
inline simdjson2_result<element> element::at(std::string_view json_pointer) const noexcept {
  // version 0.4 of simdjson2 allowed non-compliant pointers
  auto std_pointer = (json_pointer.empty() ? "" : "/") + std::string(json_pointer.begin(), json_pointer.end());
  return at_pointer(std_pointer);
}
#endif // SIMDJSON2_DISABLE_DEPRECATED_API

inline simdjson2_result<element> element::at(size_t index) const noexcept {
  return get<array>().at(index);
}
inline simdjson2_result<element> element::at_key(std::string_view key) const noexcept {
  return get<object>().at_key(key);
}
inline simdjson2_result<element> element::at_key_case_insensitive(std::string_view key) const noexcept {
  return get<object>().at_key_case_insensitive(key);
}
inline bool element::operator<(const element &other) const noexcept {
  return tape.json_index < other.tape.json_index;
}
inline bool element::operator==(const element &other) const noexcept {
  return tape.json_index == other.tape.json_index;
}

inline bool element::dump_raw_tape(std::ostream &out) const noexcept {
  SIMDJSON2_DEVELOPMENT_ASSERT(tape.usable()); // https://github.com/simdjson2/simdjson2/issues/1914
  return tape.doc->dump_raw_tape(out);
}


inline std::ostream& operator<<(std::ostream& out, element_type type) {
  switch (type) {
    case element_type::ARRAY:
      return out << "array";
    case element_type::OBJECT:
      return out << "object";
    case element_type::INT64:
      return out << "int64_t";
    case element_type::UINT64:
      return out << "uint64_t";
    case element_type::DOUBLE:
      return out << "double";
    case element_type::STRING:
      return out << "string";
    case element_type::BOOL:
      return out << "bool";
    case element_type::NULL_VALUE:
      return out << "null";
    default:
      return out << "unexpected content!!!"; // abort() usage is forbidden in the library
  }
}

} // namespace dom

} // namespace simdjson2

#endif // SIMDJSON2_ELEMENT_INL_H
