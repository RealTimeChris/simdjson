#ifndef SIMDJSON2_GENERIC_ONDEMAND_VALUE_INL_H

#ifndef SIMDJSON2_CONDITIONAL_INCLUDE
#define SIMDJSON2_GENERIC_ONDEMAND_VALUE_INL_H
#include "simdjson2/generic/ondemand/base.h"
#include "simdjson2/generic/ondemand/array.h"
#include "simdjson2/generic/ondemand/array_iterator.h"
#include "simdjson2/generic/ondemand/json_iterator.h"
#include "simdjson2/generic/ondemand/json_type.h"
#include "simdjson2/generic/ondemand/object.h"
#include "simdjson2/generic/ondemand/raw_json_string.h"
#include "simdjson2/generic/ondemand/value.h"
#endif // SIMDJSON2_CONDITIONAL_INCLUDE

namespace simdjson2 {
namespace SIMDJSON2_IMPLEMENTATION {
namespace ondemand {

simdjson2_inline value::value(const value_iterator &_iter) noexcept
  : iter{_iter}
{
}
simdjson2_inline value value::start(const value_iterator &iter) noexcept {
  return iter;
}
simdjson2_inline value value::resume(const value_iterator &iter) noexcept {
  return iter;
}

simdjson2_inline simdjson2_result<array> value::get_array() noexcept {
  return array::start(iter);
}
simdjson2_inline simdjson2_result<object> value::get_object() noexcept {
  return object::start(iter);
}
simdjson2_inline simdjson2_result<object> value::start_or_resume_object() noexcept {
  if (iter.at_start()) {
    return get_object();
  } else {
    return object::resume(iter);
  }
}

simdjson2_inline simdjson2_result<raw_json_string> value::get_raw_json_string() noexcept {
  return iter.get_raw_json_string();
}
simdjson2_inline simdjson2_result<std::string_view> value::get_string(bool allow_replacement) noexcept {
  return iter.get_string(allow_replacement);
}
template <typename string_type>
simdjson2_inline error_code value::get_string(string_type& receiver, bool allow_replacement) noexcept {
  return iter.get_string(receiver, allow_replacement);
}
simdjson2_inline simdjson2_result<std::string_view> value::get_wobbly_string() noexcept {
  return iter.get_wobbly_string();
}
simdjson2_inline simdjson2_result<double> value::get_double() noexcept {
  return iter.get_double();
}
simdjson2_inline simdjson2_result<double> value::get_double_in_string() noexcept {
  return iter.get_double_in_string();
}
simdjson2_inline simdjson2_result<uint64_t> value::get_uint64() noexcept {
  return iter.get_uint64();
}
simdjson2_inline simdjson2_result<uint64_t> value::get_uint64_in_string() noexcept {
  return iter.get_uint64_in_string();
}
simdjson2_inline simdjson2_result<int64_t> value::get_int64() noexcept {
  return iter.get_int64();
}
simdjson2_inline simdjson2_result<int64_t> value::get_int64_in_string() noexcept {
  return iter.get_int64_in_string();
}
simdjson2_inline simdjson2_result<bool> value::get_bool() noexcept {
  return iter.get_bool();
}
simdjson2_inline simdjson2_result<bool> value::is_null() noexcept {
  return iter.is_null();
}

template<> simdjson2_inline simdjson2_result<array> value::get() noexcept { return get_array(); }
template<> simdjson2_inline simdjson2_result<object> value::get() noexcept { return get_object(); }
template<> simdjson2_inline simdjson2_result<raw_json_string> value::get() noexcept { return get_raw_json_string(); }
template<> simdjson2_inline simdjson2_result<std::string_view> value::get() noexcept { return get_string(false); }
template<> simdjson2_inline simdjson2_result<number> value::get() noexcept { return get_number(); }
template<> simdjson2_inline simdjson2_result<double> value::get() noexcept { return get_double(); }
template<> simdjson2_inline simdjson2_result<uint64_t> value::get() noexcept { return get_uint64(); }
template<> simdjson2_inline simdjson2_result<int64_t> value::get() noexcept { return get_int64(); }
template<> simdjson2_inline simdjson2_result<bool> value::get() noexcept { return get_bool(); }


template<> simdjson2_inline error_code value::get(array& out) noexcept { return get_array().get(out); }
template<> simdjson2_inline error_code value::get(object& out) noexcept { return get_object().get(out); }
template<> simdjson2_inline error_code value::get(raw_json_string& out) noexcept { return get_raw_json_string().get(out); }
template<> simdjson2_inline error_code value::get(std::string_view& out) noexcept { return get_string(false).get(out); }
template<> simdjson2_inline error_code value::get(number& out) noexcept { return get_number().get(out); }
template<> simdjson2_inline error_code value::get(double& out) noexcept { return get_double().get(out); }
template<> simdjson2_inline error_code value::get(uint64_t& out) noexcept { return get_uint64().get(out); }
template<> simdjson2_inline error_code value::get(int64_t& out) noexcept { return get_int64().get(out); }
template<> simdjson2_inline error_code value::get(bool& out) noexcept { return get_bool().get(out); }

#if SIMDJSON2_EXCEPTIONS
template <class T>
simdjson2_inline value::operator T() noexcept(false) {
  return get<T>();
}
simdjson2_inline value::operator array() noexcept(false) {
  return get_array();
}
simdjson2_inline value::operator object() noexcept(false) {
  return get_object();
}
simdjson2_inline value::operator uint64_t() noexcept(false) {
  return get_uint64();
}
simdjson2_inline value::operator int64_t() noexcept(false) {
  return get_int64();
}
simdjson2_inline value::operator double() noexcept(false) {
  return get_double();
}
simdjson2_inline value::operator std::string_view() noexcept(false) {
  return get_string(false);
}
simdjson2_inline value::operator raw_json_string() noexcept(false) {
  return get_raw_json_string();
}
simdjson2_inline value::operator bool() noexcept(false) {
  return get_bool();
}
#endif

simdjson2_inline simdjson2_result<array_iterator> value::begin() & noexcept {
  return get_array().begin();
}
simdjson2_inline simdjson2_result<array_iterator> value::end() & noexcept {
  return {};
}
simdjson2_inline simdjson2_result<size_t> value::count_elements() & noexcept {
  simdjson2_result<size_t> answer;
  auto a = get_array();
  answer = a.count_elements();
  // count_elements leaves you pointing inside the array, at the first element.
  // We need to move back so that the user can create a new array (which requires that
  // we point at '[').
  iter.move_at_start();
  return answer;
}
simdjson2_inline simdjson2_result<size_t> value::count_fields() & noexcept {
  simdjson2_result<size_t> answer;
  auto a = get_object();
  answer = a.count_fields();
  iter.move_at_start();
  return answer;
}
simdjson2_inline simdjson2_result<value> value::at(size_t index) noexcept {
  auto a = get_array();
  return a.at(index);
}

simdjson2_inline simdjson2_result<value> value::find_field(std::string_view key) noexcept {
  return start_or_resume_object().find_field(key);
}
simdjson2_inline simdjson2_result<value> value::find_field(const char *key) noexcept {
  return start_or_resume_object().find_field(key);
}

simdjson2_inline simdjson2_result<value> value::find_field_unordered(std::string_view key) noexcept {
  return start_or_resume_object().find_field_unordered(key);
}
simdjson2_inline simdjson2_result<value> value::find_field_unordered(const char *key) noexcept {
  return start_or_resume_object().find_field_unordered(key);
}

simdjson2_inline simdjson2_result<value> value::operator[](std::string_view key) noexcept {
  return start_or_resume_object()[key];
}
simdjson2_inline simdjson2_result<value> value::operator[](const char *key) noexcept {
  return start_or_resume_object()[key];
}

simdjson2_inline simdjson2_result<json_type> value::type() noexcept {
  return iter.type();
}

simdjson2_inline simdjson2_result<bool> value::is_scalar() noexcept {
  json_type this_type;
  auto error = type().get(this_type);
  if(error) { return error; }
  return ! ((this_type == json_type::array) || (this_type == json_type::object));
}

simdjson2_inline simdjson2_result<bool> value::is_string() noexcept {
  json_type this_type;
  auto error = type().get(this_type);
  if(error) { return error; }
  return (this_type == json_type::string);
}


simdjson2_inline bool value::is_negative() noexcept {
  return iter.is_negative();
}

simdjson2_inline simdjson2_result<bool> value::is_integer() noexcept {
  return iter.is_integer();
}
simdjson2_warn_unused simdjson2_inline simdjson2_result<number_type> value::get_number_type() noexcept {
  return iter.get_number_type();
}
simdjson2_warn_unused simdjson2_inline simdjson2_result<number> value::get_number() noexcept {
  return iter.get_number();
}

simdjson2_inline std::string_view value::raw_json_token() noexcept {
  return std::string_view(reinterpret_cast<const char*>(iter.peek_start()), iter.peek_start_length());
}

simdjson2_inline simdjson2_result<std::string_view> value::raw_json() noexcept {
  json_type t;
  SIMDJSON2_TRY(type().get(t));
  switch (t)
  {
    case json_type::array: {
      ondemand::array array;
      SIMDJSON2_TRY(get_array().get(array));
      return array.raw_json();
    }
    case json_type::object: {
      ondemand::object object;
      SIMDJSON2_TRY(get_object().get(object));
      return object.raw_json();
    }
    default:
      return raw_json_token();
  }
}

simdjson2_inline simdjson2_result<const char *> value::current_location() noexcept {
  return iter.json_iter().current_location();
}

simdjson2_inline int32_t value::current_depth() const noexcept{
  return iter.json_iter().depth();
}

inline bool is_pointer_well_formed(std::string_view json_pointer) noexcept {
  if (simdjson2_unlikely(json_pointer.empty())) { // can't be
    return false;
  }
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

simdjson2_inline simdjson2_result<value> value::at_pointer(std::string_view json_pointer) noexcept {
  json_type t;
  SIMDJSON2_TRY(type().get(t));
  switch (t)
  {
    case json_type::array:
      return (*this).get_array().at_pointer(json_pointer);
    case json_type::object:
      return (*this).get_object().at_pointer(json_pointer);
    default:
      // a non-empty string can be invalid, or accessing a primitive (issue 2154)
      if (is_pointer_well_formed(json_pointer)) {
        return NO_SUCH_FIELD;
      }
      return INVALID_JSON_POINTER;
  }
}

simdjson2_inline simdjson2_result<value> value::at_path(std::string_view json_path) noexcept {
  json_type t;
  SIMDJSON2_TRY(type().get(t));
  switch (t) {
  case json_type::array:
      return (*this).get_array().at_path(json_path);
  case json_type::object:
      return (*this).get_object().at_path(json_path);
  default:
      return INVALID_JSON_POINTER;
  }
}

} // namespace ondemand
} // namespace SIMDJSON2_IMPLEMENTATION
} // namespace simdjson2

namespace simdjson2 {

simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::value>::simdjson2_result(
  SIMDJSON2_IMPLEMENTATION::ondemand::value &&value
) noexcept :
    implementation_simdjson2_result_base<SIMDJSON2_IMPLEMENTATION::ondemand::value>(
      std::forward<SIMDJSON2_IMPLEMENTATION::ondemand::value>(value)
    )
{
}
simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::value>::simdjson2_result(
  error_code error
) noexcept :
    implementation_simdjson2_result_base<SIMDJSON2_IMPLEMENTATION::ondemand::value>(error)
{
}
simdjson2_inline simdjson2_result<size_t> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::value>::count_elements() & noexcept {
  if (error()) { return error(); }
  return first.count_elements();
}
simdjson2_inline simdjson2_result<size_t> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::value>::count_fields() & noexcept {
  if (error()) { return error(); }
  return first.count_fields();
}
simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::value> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::value>::at(size_t index) noexcept {
  if (error()) { return error(); }
  return first.at(index);
}
simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::array_iterator> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::value>::begin() & noexcept {
  if (error()) { return error(); }
  return first.begin();
}
simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::array_iterator> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::value>::end() & noexcept {
  if (error()) { return error(); }
  return {};
}

simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::value> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::value>::find_field(std::string_view key) noexcept {
  if (error()) { return error(); }
  return first.find_field(key);
}
simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::value> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::value>::find_field(const char *key) noexcept {
  if (error()) { return error(); }
  return first.find_field(key);
}

simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::value> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::value>::find_field_unordered(std::string_view key) noexcept {
  if (error()) { return error(); }
  return first.find_field_unordered(key);
}
simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::value> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::value>::find_field_unordered(const char *key) noexcept {
  if (error()) { return error(); }
  return first.find_field_unordered(key);
}

simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::value> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::value>::operator[](std::string_view key) noexcept {
  if (error()) { return error(); }
  return first[key];
}
simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::value> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::value>::operator[](const char *key) noexcept {
  if (error()) { return error(); }
  return first[key];
}

simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::array> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::value>::get_array() noexcept {
  if (error()) { return error(); }
  return first.get_array();
}
simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::object> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::value>::get_object() noexcept {
  if (error()) { return error(); }
  return first.get_object();
}
simdjson2_inline simdjson2_result<uint64_t> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::value>::get_uint64() noexcept {
  if (error()) { return error(); }
  return first.get_uint64();
}
simdjson2_inline simdjson2_result<uint64_t> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::value>::get_uint64_in_string() noexcept {
  if (error()) { return error(); }
  return first.get_uint64_in_string();
}
simdjson2_inline simdjson2_result<int64_t> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::value>::get_int64() noexcept {
  if (error()) { return error(); }
  return first.get_int64();
}
simdjson2_inline simdjson2_result<int64_t> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::value>::get_int64_in_string() noexcept {
  if (error()) { return error(); }
  return first.get_int64_in_string();
}
simdjson2_inline simdjson2_result<double> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::value>::get_double() noexcept {
  if (error()) { return error(); }
  return first.get_double();
}
simdjson2_inline simdjson2_result<double> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::value>::get_double_in_string() noexcept {
  if (error()) { return error(); }
  return first.get_double_in_string();
}
simdjson2_inline simdjson2_result<std::string_view> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::value>::get_string(bool allow_replacement) noexcept {
  if (error()) { return error(); }
  return first.get_string(allow_replacement);
}
template <typename string_type>
simdjson2_inline error_code simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::value>::get_string(string_type& receiver, bool allow_replacement) noexcept {
  if (error()) { return error(); }
  return first.get_string(receiver, allow_replacement);
}
simdjson2_inline simdjson2_result<std::string_view> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::value>::get_wobbly_string() noexcept {
  if (error()) { return error(); }
  return first.get_wobbly_string();
}
simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::raw_json_string> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::value>::get_raw_json_string() noexcept {
  if (error()) { return error(); }
  return first.get_raw_json_string();
}
simdjson2_inline simdjson2_result<bool> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::value>::get_bool() noexcept {
  if (error()) { return error(); }
  return first.get_bool();
}
simdjson2_inline simdjson2_result<bool> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::value>::is_null() noexcept {
  if (error()) { return error(); }
  return first.is_null();
}

template<> simdjson2_inline error_code simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::value>::get<SIMDJSON2_IMPLEMENTATION::ondemand::value>(SIMDJSON2_IMPLEMENTATION::ondemand::value &out) noexcept {
  if (error()) { return error(); }
  out = first;
  return SUCCESS;
}

template<typename T> simdjson2_inline simdjson2_result<T> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::value>::get() noexcept {
  if (error()) { return error(); }
  return first.get<T>();
}
template<typename T> simdjson2_inline error_code simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::value>::get(T &out) noexcept {
  if (error()) { return error(); }
  return first.get<T>(out);
}

template<> simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::value> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::value>::get<SIMDJSON2_IMPLEMENTATION::ondemand::value>() noexcept  {
  if (error()) { return error(); }
  return std::move(first);
}

simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::json_type> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::value>::type() noexcept {
  if (error()) { return error(); }
  return first.type();
}
simdjson2_inline simdjson2_result<bool> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::value>::is_scalar() noexcept {
  if (error()) { return error(); }
  return first.is_scalar();
}
simdjson2_inline simdjson2_result<bool> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::value>::is_string() noexcept {
  if (error()) { return error(); }
  return first.is_string();
}
simdjson2_inline simdjson2_result<bool> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::value>::is_negative() noexcept {
  if (error()) { return error(); }
  return first.is_negative();
}
simdjson2_inline simdjson2_result<bool> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::value>::is_integer() noexcept {
  if (error()) { return error(); }
  return first.is_integer();
}
simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::number_type> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::value>::get_number_type() noexcept {
  if (error()) { return error(); }
  return first.get_number_type();
}
simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::number> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::value>::get_number() noexcept {
  if (error()) { return error(); }
  return first.get_number();
}
#if SIMDJSON2_EXCEPTIONS
template <class T>
simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::value>::operator T() noexcept(false) {
  if (error()) { throw simdjson2_error(error()); }
  return first.get<T>();
}
simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::value>::operator SIMDJSON2_IMPLEMENTATION::ondemand::array() noexcept(false) {
  if (error()) { throw simdjson2_error(error()); }
  return first;
}
simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::value>::operator SIMDJSON2_IMPLEMENTATION::ondemand::object() noexcept(false) {
  if (error()) { throw simdjson2_error(error()); }
  return first;
}
simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::value>::operator uint64_t() noexcept(false) {
  if (error()) { throw simdjson2_error(error()); }
  return first;
}
simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::value>::operator int64_t() noexcept(false) {
  if (error()) { throw simdjson2_error(error()); }
  return first;
}
simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::value>::operator double() noexcept(false) {
  if (error()) { throw simdjson2_error(error()); }
  return first;
}
simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::value>::operator std::string_view() noexcept(false) {
  if (error()) { throw simdjson2_error(error()); }
  return first;
}
simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::value>::operator SIMDJSON2_IMPLEMENTATION::ondemand::raw_json_string() noexcept(false) {
  if (error()) { throw simdjson2_error(error()); }
  return first;
}
simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::value>::operator bool() noexcept(false) {
  if (error()) { throw simdjson2_error(error()); }
  return first;
}
#endif

simdjson2_inline simdjson2_result<std::string_view> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::value>::raw_json_token() noexcept {
  if (error()) { return error(); }
  return first.raw_json_token();
}

simdjson2_inline simdjson2_result<std::string_view> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::value>::raw_json() noexcept {
  if (error()) { return error(); }
  return first.raw_json();
}

simdjson2_inline simdjson2_result<const char *> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::value>::current_location() noexcept {
  if (error()) { return error(); }
  return first.current_location();
}

simdjson2_inline simdjson2_result<int32_t> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::value>::current_depth() const noexcept {
  if (error()) { return error(); }
  return first.current_depth();
}

simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::value> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::value>::at_pointer(
    std::string_view json_pointer) noexcept {
  if (error()) {
      return error();
  }
  return first.at_pointer(json_pointer);
}

simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::value> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::value>::at_path(
      std::string_view json_path) noexcept {
  if (error()) {
    return error();
  }
  return first.at_path(json_path);
}

} // namespace simdjson2

#endif // SIMDJSON2_GENERIC_ONDEMAND_VALUE_INL_H
