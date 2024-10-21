#ifndef SIMDJSON2_GENERIC_ONDEMAND_DOCUMENT_INL_H

#ifndef SIMDJSON2_CONDITIONAL_INCLUDE
#define SIMDJSON2_GENERIC_ONDEMAND_DOCUMENT_INL_H
#include "simdjson2/generic/ondemand/base.h"
#include "simdjson2/generic/ondemand/array_iterator.h"
#include "simdjson2/generic/ondemand/document.h"
#include "simdjson2/generic/ondemand/json_type.h"
#include "simdjson2/generic/ondemand/raw_json_string.h"
#include "simdjson2/generic/ondemand/value.h"
#include "simdjson2/generic/ondemand/value-inl.h"
#include "simdjson2/generic/ondemand/array-inl.h"
#include "simdjson2/generic/ondemand/json_iterator-inl.h"
#include "simdjson2/generic/ondemand/object-inl.h"
#include "simdjson2/generic/ondemand/value_iterator-inl.h"
#include "simdjson2/generic/ondemand/deserialize.h"
#endif // SIMDJSON2_CONDITIONAL_INCLUDE

namespace simdjson2 {
namespace SIMDJSON2_IMPLEMENTATION {
namespace ondemand {

simdjson2_inline document::document(ondemand::json_iterator &&_iter) noexcept
  : iter{std::forward<json_iterator>(_iter)}
{
  logger::log_start_value(iter, "document");
}

simdjson2_inline document document::start(json_iterator &&iter) noexcept {
  return document(std::forward<json_iterator>(iter));
}

inline void document::rewind() noexcept {
  iter.rewind();
}

inline std::string document::to_debug_string() noexcept {
  return iter.to_string();
}

inline simdjson2_result<const char *> document::current_location() const noexcept {
  return iter.current_location();
}

inline int32_t document::current_depth() const noexcept {
  return iter.depth();
}

inline bool document::at_end() const noexcept {
  return iter.at_end();
}


inline bool document::is_alive() noexcept {
  return iter.is_alive();
}
simdjson2_inline value_iterator document::resume_value_iterator() noexcept {
  return value_iterator(&iter, 1, iter.root_position());
}
simdjson2_inline value_iterator document::get_root_value_iterator() noexcept {
  return resume_value_iterator();
}
simdjson2_inline simdjson2_result<object> document::start_or_resume_object() noexcept {
  if (iter.at_root()) {
    return get_object();
  } else {
    return object::resume(resume_value_iterator());
  }
}
simdjson2_inline simdjson2_result<value> document::get_value() noexcept {
  // Make sure we start any arrays or objects before returning, so that start_root_<object/array>()
  // gets called.

  // It is the convention throughout the code that  the macro `SIMDJSON2_DEVELOPMENT_CHECKS` determines whether
  // we check for OUT_OF_ORDER_ITERATION. Proper on::demand code should never trigger this error.
#if SIMDJSON2_DEVELOPMENT_CHECKS
  if (!iter.at_root()) { return OUT_OF_ORDER_ITERATION; }
#endif
  // assert_at_root() serves two purposes: in Debug mode, whether or not
  // SIMDJSON2_DEVELOPMENT_CHECKS is set or not, it checks that we are at the root of
  // the document (this will typically be redundant). In release mode, it generates
  // SIMDJSON2_ASSUME statements to allow the compiler to make assumptions.
  iter.assert_at_root();
  switch (*iter.peek()) {
    case '[': {
      // The following lines check that the document ends with ].
      auto value_iterator = get_root_value_iterator();
      auto error = value_iterator.check_root_array();
      if(error) { return error; }
      return value(get_root_value_iterator());
    }
    case '{': {
      // The following lines would check that the document ends with }.
      auto value_iterator = get_root_value_iterator();
      auto error = value_iterator.check_root_object();
      if(error) { return error; }
      return value(get_root_value_iterator());
    }
    default:
      // Unfortunately, scalar documents are a special case in simdjson2 and they cannot
      // be safely converted to value instances.
      return SCALAR_DOCUMENT_AS_VALUE;
  }
}
simdjson2_inline simdjson2_result<array> document::get_array() & noexcept {
  auto value = get_root_value_iterator();
  return array::start_root(value);
}
simdjson2_inline simdjson2_result<object> document::get_object() & noexcept {
  auto value = get_root_value_iterator();
  return object::start_root(value);
}

/**
 * We decided that calling 'get_double()' on the JSON document '1.233 blabla' should
 * give an error, so we check for trailing content. We want to disallow trailing
 * content.
 * Thus, in several implementations below, we pass a 'true' parameter value to
 * a get_root_value_iterator() method: this indicates that we disallow trailing content.
 */

simdjson2_inline simdjson2_result<uint64_t> document::get_uint64() noexcept {
  return get_root_value_iterator().get_root_uint64(true);
}
simdjson2_inline simdjson2_result<uint64_t> document::get_uint64_in_string() noexcept {
  return get_root_value_iterator().get_root_uint64_in_string(true);
}
simdjson2_inline simdjson2_result<int64_t> document::get_int64() noexcept {
  return get_root_value_iterator().get_root_int64(true);
}
simdjson2_inline simdjson2_result<int64_t> document::get_int64_in_string() noexcept {
  return get_root_value_iterator().get_root_int64_in_string(true);
}
simdjson2_inline simdjson2_result<double> document::get_double() noexcept {
  return get_root_value_iterator().get_root_double(true);
}
simdjson2_inline simdjson2_result<double> document::get_double_in_string() noexcept {
  return get_root_value_iterator().get_root_double_in_string(true);
}
simdjson2_inline simdjson2_result<std::string_view> document::get_string(bool allow_replacement) noexcept {
  return get_root_value_iterator().get_root_string(true, allow_replacement);
}
template <typename string_type>
simdjson2_inline error_code document::get_string(string_type& receiver, bool allow_replacement) noexcept {
  return get_root_value_iterator().get_root_string(receiver, true, allow_replacement);
}
simdjson2_inline simdjson2_result<std::string_view> document::get_wobbly_string() noexcept {
  return get_root_value_iterator().get_root_wobbly_string(true);
}
simdjson2_inline simdjson2_result<raw_json_string> document::get_raw_json_string() noexcept {
  return get_root_value_iterator().get_root_raw_json_string(true);
}
simdjson2_inline simdjson2_result<bool> document::get_bool() noexcept {
  return get_root_value_iterator().get_root_bool(true);
}
simdjson2_inline simdjson2_result<bool> document::is_null() noexcept {
  return get_root_value_iterator().is_root_null(true);
}

template<> simdjson2_inline simdjson2_result<array> document::get() & noexcept { return get_array(); }
template<> simdjson2_inline simdjson2_result<object> document::get() & noexcept { return get_object(); }
template<> simdjson2_inline simdjson2_result<raw_json_string> document::get() & noexcept { return get_raw_json_string(); }
template<> simdjson2_inline simdjson2_result<std::string_view> document::get() & noexcept { return get_string(false); }
template<> simdjson2_inline simdjson2_result<double> document::get() & noexcept { return get_double(); }
template<> simdjson2_inline simdjson2_result<uint64_t> document::get() & noexcept { return get_uint64(); }
template<> simdjson2_inline simdjson2_result<int64_t> document::get() & noexcept { return get_int64(); }
template<> simdjson2_inline simdjson2_result<bool> document::get() & noexcept { return get_bool(); }
template<> simdjson2_inline simdjson2_result<value> document::get() & noexcept { return get_value(); }

template<> simdjson2_inline error_code document::get(array& out) & noexcept { return get_array().get(out); }
template<> simdjson2_inline error_code document::get(object& out) & noexcept { return get_object().get(out); }
template<> simdjson2_inline error_code document::get(raw_json_string& out) & noexcept { return get_raw_json_string().get(out); }
template<> simdjson2_inline error_code document::get(std::string_view& out) & noexcept { return get_string(false).get(out); }
template<> simdjson2_inline error_code document::get(double& out) & noexcept { return get_double().get(out); }
template<> simdjson2_inline error_code document::get(uint64_t& out) & noexcept { return get_uint64().get(out); }
template<> simdjson2_inline error_code document::get(int64_t& out) & noexcept { return get_int64().get(out); }
template<> simdjson2_inline error_code document::get(bool& out) & noexcept { return get_bool().get(out); }
template<> simdjson2_inline error_code document::get(value& out) & noexcept { return get_value().get(out); }

template<> simdjson2_deprecated simdjson2_inline simdjson2_result<raw_json_string> document::get() && noexcept { return get_raw_json_string(); }
template<> simdjson2_deprecated simdjson2_inline simdjson2_result<std::string_view> document::get() && noexcept { return get_string(false); }
template<> simdjson2_deprecated simdjson2_inline simdjson2_result<double> document::get() && noexcept { return std::forward<document>(*this).get_double(); }
template<> simdjson2_deprecated simdjson2_inline simdjson2_result<uint64_t> document::get() && noexcept { return std::forward<document>(*this).get_uint64(); }
template<> simdjson2_deprecated simdjson2_inline simdjson2_result<int64_t> document::get() && noexcept { return std::forward<document>(*this).get_int64(); }
template<> simdjson2_deprecated simdjson2_inline simdjson2_result<bool> document::get() && noexcept { return std::forward<document>(*this).get_bool(); }
template<> simdjson2_deprecated simdjson2_inline simdjson2_result<value> document::get() && noexcept { return get_value(); }

#if SIMDJSON2_EXCEPTIONS
template <class T>
simdjson2_deprecated simdjson2_inline document::operator T() && noexcept(false) { return get<T>(); }
template <class T>
simdjson2_inline document::operator T() & noexcept(false) { return get<T>(); }
simdjson2_inline document::operator array() & noexcept(false) { return get_array(); }
simdjson2_inline document::operator object() & noexcept(false) { return get_object(); }
simdjson2_inline document::operator uint64_t() noexcept(false) { return get_uint64(); }
simdjson2_inline document::operator int64_t() noexcept(false) { return get_int64(); }
simdjson2_inline document::operator double() noexcept(false) { return get_double(); }
simdjson2_inline document::operator std::string_view() noexcept(false) { return get_string(false); }
simdjson2_inline document::operator raw_json_string() noexcept(false) { return get_raw_json_string(); }
simdjson2_inline document::operator bool() noexcept(false) { return get_bool(); }
simdjson2_inline document::operator value() noexcept(false) { return get_value(); }

#endif
simdjson2_inline simdjson2_result<size_t> document::count_elements() & noexcept {
  auto a = get_array();
  simdjson2_result<size_t> answer = a.count_elements();
  /* If there was an array, we are now left pointing at its first element. */
  if(answer.error() == SUCCESS) { rewind(); }
  return answer;
}
simdjson2_inline simdjson2_result<size_t> document::count_fields() & noexcept {
  auto a = get_object();
  simdjson2_result<size_t> answer = a.count_fields();
  /* If there was an object, we are now left pointing at its first element. */
  if(answer.error() == SUCCESS) { rewind(); }
  return answer;
}
simdjson2_inline simdjson2_result<value> document::at(size_t index) & noexcept {
  auto a = get_array();
  return a.at(index);
}
simdjson2_inline simdjson2_result<array_iterator> document::begin() & noexcept {
  return get_array().begin();
}
simdjson2_inline simdjson2_result<array_iterator> document::end() & noexcept {
  return {};
}

simdjson2_inline simdjson2_result<value> document::find_field(std::string_view key) & noexcept {
  return start_or_resume_object().find_field(key);
}
simdjson2_inline simdjson2_result<value> document::find_field(const char *key) & noexcept {
  return start_or_resume_object().find_field(key);
}
simdjson2_inline simdjson2_result<value> document::find_field_unordered(std::string_view key) & noexcept {
  return start_or_resume_object().find_field_unordered(key);
}
simdjson2_inline simdjson2_result<value> document::find_field_unordered(const char *key) & noexcept {
  return start_or_resume_object().find_field_unordered(key);
}
simdjson2_inline simdjson2_result<value> document::operator[](std::string_view key) & noexcept {
  return start_or_resume_object()[key];
}
simdjson2_inline simdjson2_result<value> document::operator[](const char *key) & noexcept {
  return start_or_resume_object()[key];
}

simdjson2_inline error_code document::consume() noexcept {
  auto error = iter.skip_child(0);
  if(error) { iter.abandon(); }
  return error;
}

simdjson2_inline simdjson2_result<std::string_view> document::raw_json() noexcept {
  auto _iter = get_root_value_iterator();
  const uint8_t * starting_point{_iter.peek_start()};
  auto error = consume();
  if(error) { return error; }
  // After 'consume()', we could be left pointing just beyond the document, but that
  // is ok because we are not going to dereference the final pointer position, we just
  // use it to compute the length in bytes.
  const uint8_t * final_point{iter.unsafe_pointer()};
  return std::string_view(reinterpret_cast<const char*>(starting_point), size_t(final_point - starting_point));
}

simdjson2_inline simdjson2_result<json_type> document::type() noexcept {
  return get_root_value_iterator().type();
}

simdjson2_inline simdjson2_result<bool> document::is_scalar() noexcept {
  json_type this_type;
  auto error = type().get(this_type);
  if(error) { return error; }
  return ! ((this_type == json_type::array) || (this_type == json_type::object));
}

simdjson2_inline simdjson2_result<bool> document::is_string() noexcept {
  json_type this_type;
  auto error = type().get(this_type);
  if(error) { return error; }
  return (this_type == json_type::string);
}

simdjson2_inline bool document::is_negative() noexcept {
  return get_root_value_iterator().is_root_negative();
}

simdjson2_inline simdjson2_result<bool> document::is_integer() noexcept {
  return get_root_value_iterator().is_root_integer(true);
}

simdjson2_inline simdjson2_result<number_type> document::get_number_type() noexcept {
  return get_root_value_iterator().get_root_number_type(true);
}

simdjson2_inline simdjson2_result<number> document::get_number() noexcept {
  return get_root_value_iterator().get_root_number(true);
}


simdjson2_inline simdjson2_result<std::string_view> document::raw_json_token() noexcept {
  auto _iter = get_root_value_iterator();
  return std::string_view(reinterpret_cast<const char*>(_iter.peek_start()), _iter.peek_root_length());
}

simdjson2_inline simdjson2_result<value> document::at_pointer(std::string_view json_pointer) noexcept {
  rewind(); // Rewind the document each time at_pointer is called
  if (json_pointer.empty()) {
    return this->get_value();
  }
  json_type t;
  SIMDJSON2_TRY(type().get(t));
  switch (t)
  {
    case json_type::array:
      return (*this).get_array().at_pointer(json_pointer);
    case json_type::object:
      return (*this).get_object().at_pointer(json_pointer);
    default:
      return INVALID_JSON_POINTER;
  }
}

simdjson2_inline simdjson2_result<value> document::at_path(std::string_view json_path) noexcept {
  rewind(); // Rewind the document each time at_pointer is called
  if (json_path.empty()) {
      return this->get_value();
  }
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

simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document>::simdjson2_result(
  SIMDJSON2_IMPLEMENTATION::ondemand::document &&value
) noexcept :
    implementation_simdjson2_result_base<SIMDJSON2_IMPLEMENTATION::ondemand::document>(
      std::forward<SIMDJSON2_IMPLEMENTATION::ondemand::document>(value)
    )
{
}
simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document>::simdjson2_result(
  error_code error
) noexcept :
    implementation_simdjson2_result_base<SIMDJSON2_IMPLEMENTATION::ondemand::document>(
      error
    )
{
}
simdjson2_inline simdjson2_result<size_t> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document>::count_elements() & noexcept {
  if (error()) { return error(); }
  return first.count_elements();
}
simdjson2_inline simdjson2_result<size_t> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document>::count_fields() & noexcept {
  if (error()) { return error(); }
  return first.count_fields();
}
simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::value> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document>::at(size_t index) & noexcept {
  if (error()) { return error(); }
  return first.at(index);
}
simdjson2_inline error_code simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document>::rewind() noexcept {
  if (error()) { return error(); }
  first.rewind();
  return SUCCESS;
}
simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::array_iterator> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document>::begin() & noexcept {
  if (error()) { return error(); }
  return first.begin();
}
simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::array_iterator> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document>::end() & noexcept {
  return {};
}
simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::value> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document>::find_field_unordered(std::string_view key) & noexcept {
  if (error()) { return error(); }
  return first.find_field_unordered(key);
}
simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::value> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document>::find_field_unordered(const char *key) & noexcept {
  if (error()) { return error(); }
  return first.find_field_unordered(key);
}
simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::value> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document>::operator[](std::string_view key) & noexcept {
  if (error()) { return error(); }
  return first[key];
}
simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::value> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document>::operator[](const char *key) & noexcept {
  if (error()) { return error(); }
  return first[key];
}
simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::value> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document>::find_field(std::string_view key) & noexcept {
  if (error()) { return error(); }
  return first.find_field(key);
}
simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::value> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document>::find_field(const char *key) & noexcept {
  if (error()) { return error(); }
  return first.find_field(key);
}
simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::array> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document>::get_array() & noexcept {
  if (error()) { return error(); }
  return first.get_array();
}
simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::object> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document>::get_object() & noexcept {
  if (error()) { return error(); }
  return first.get_object();
}
simdjson2_inline simdjson2_result<uint64_t> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document>::get_uint64() noexcept {
  if (error()) { return error(); }
  return first.get_uint64();
}
simdjson2_inline simdjson2_result<uint64_t> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document>::get_uint64_in_string() noexcept {
  if (error()) { return error(); }
  return first.get_uint64_in_string();
}
simdjson2_inline simdjson2_result<int64_t> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document>::get_int64() noexcept {
  if (error()) { return error(); }
  return first.get_int64();
}
simdjson2_inline simdjson2_result<int64_t> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document>::get_int64_in_string() noexcept {
  if (error()) { return error(); }
  return first.get_int64_in_string();
}
simdjson2_inline simdjson2_result<double> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document>::get_double() noexcept {
  if (error()) { return error(); }
  return first.get_double();
}
simdjson2_inline simdjson2_result<double> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document>::get_double_in_string() noexcept {
  if (error()) { return error(); }
  return first.get_double_in_string();
}
simdjson2_inline simdjson2_result<std::string_view> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document>::get_string(bool allow_replacement) noexcept {
  if (error()) { return error(); }
  return first.get_string(allow_replacement);
}
template <typename string_type>
simdjson2_inline error_code simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document>::get_string(string_type& receiver, bool allow_replacement) noexcept {
  if (error()) { return error(); }
  return first.get_string(receiver, allow_replacement);
}
simdjson2_inline simdjson2_result<std::string_view> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document>::get_wobbly_string() noexcept {
  if (error()) { return error(); }
  return first.get_wobbly_string();
}
simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::raw_json_string> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document>::get_raw_json_string() noexcept {
  if (error()) { return error(); }
  return first.get_raw_json_string();
}
simdjson2_inline simdjson2_result<bool> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document>::get_bool() noexcept {
  if (error()) { return error(); }
  return first.get_bool();
}
simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::value> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document>::get_value() noexcept {
  if (error()) { return error(); }
  return first.get_value();
}
simdjson2_inline simdjson2_result<bool> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document>::is_null() noexcept {
  if (error()) { return error(); }
  return first.is_null();
}

template<typename T>
simdjson2_inline simdjson2_result<T> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document>::get() & noexcept {
  if (error()) { return error(); }
  return first.get<T>();
}
template<typename T>
simdjson2_deprecated simdjson2_inline simdjson2_result<T> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document>::get() && noexcept {
  if (error()) { return error(); }
  return std::forward<SIMDJSON2_IMPLEMENTATION::ondemand::document>(first).get<T>();
}
template<typename T>
simdjson2_inline error_code simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document>::get(T &out) & noexcept {
  if (error()) { return error(); }
  return first.get<T>(out);
}
template<typename T>
simdjson2_inline error_code simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document>::get(T &out) && noexcept {
  if (error()) { return error(); }
  return std::forward<SIMDJSON2_IMPLEMENTATION::ondemand::document>(first).get<T>(out);
}

template<> simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document>::get<SIMDJSON2_IMPLEMENTATION::ondemand::document>() & noexcept = delete;
template<> simdjson2_deprecated simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document>::get<SIMDJSON2_IMPLEMENTATION::ondemand::document>() && noexcept {
  if (error()) { return error(); }
  return std::forward<SIMDJSON2_IMPLEMENTATION::ondemand::document>(first);
}
template<> simdjson2_inline error_code simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document>::get<SIMDJSON2_IMPLEMENTATION::ondemand::document>(SIMDJSON2_IMPLEMENTATION::ondemand::document &out) & noexcept = delete;
template<> simdjson2_inline error_code simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document>::get<SIMDJSON2_IMPLEMENTATION::ondemand::document>(SIMDJSON2_IMPLEMENTATION::ondemand::document &out) && noexcept {
  if (error()) { return error(); }
  out = std::forward<SIMDJSON2_IMPLEMENTATION::ondemand::document>(first);
  return SUCCESS;
}

simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::json_type> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document>::type() noexcept {
  if (error()) { return error(); }
  return first.type();
}

simdjson2_inline simdjson2_result<bool> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document>::is_scalar() noexcept {
  if (error()) { return error(); }
  return first.is_scalar();
}

simdjson2_inline simdjson2_result<bool> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document>::is_string() noexcept {
  if (error()) { return error(); }
  return first.is_string();
}

simdjson2_inline bool simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document>::is_negative() noexcept {
  if (error()) { return error(); }
  return first.is_negative();
}

simdjson2_inline simdjson2_result<bool> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document>::is_integer() noexcept {
  if (error()) { return error(); }
  return first.is_integer();
}

simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::number_type> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document>::get_number_type() noexcept {
  if (error()) { return error(); }
  return first.get_number_type();
}

simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::number> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document>::get_number() noexcept {
  if (error()) { return error(); }
  return first.get_number();
}


#if SIMDJSON2_EXCEPTIONS
template <class T, typename std::enable_if<std::is_same<T, SIMDJSON2_IMPLEMENTATION::ondemand::document>::value == false>::type>
simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document>::operator T() noexcept(false) {
  if (error()) { throw simdjson2_error(error()); }
  return first;
}
simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document>::operator SIMDJSON2_IMPLEMENTATION::ondemand::array() & noexcept(false) {
  if (error()) { throw simdjson2_error(error()); }
  return first;
}
simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document>::operator SIMDJSON2_IMPLEMENTATION::ondemand::object() & noexcept(false) {
  if (error()) { throw simdjson2_error(error()); }
  return first;
}
simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document>::operator uint64_t() noexcept(false) {
  if (error()) { throw simdjson2_error(error()); }
  return first;
}
simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document>::operator int64_t() noexcept(false) {
  if (error()) { throw simdjson2_error(error()); }
  return first;
}
simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document>::operator double() noexcept(false) {
  if (error()) { throw simdjson2_error(error()); }
  return first;
}
simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document>::operator std::string_view() noexcept(false) {
  if (error()) { throw simdjson2_error(error()); }
  return first;
}
simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document>::operator SIMDJSON2_IMPLEMENTATION::ondemand::raw_json_string() noexcept(false) {
  if (error()) { throw simdjson2_error(error()); }
  return first;
}
simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document>::operator bool() noexcept(false) {
  if (error()) { throw simdjson2_error(error()); }
  return first;
}
simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document>::operator SIMDJSON2_IMPLEMENTATION::ondemand::value() noexcept(false) {
  if (error()) { throw simdjson2_error(error()); }
  return first;
}
#endif


simdjson2_inline simdjson2_result<const char *> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document>::current_location() noexcept {
  if (error()) { return error(); }
  return first.current_location();
}

simdjson2_inline bool simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document>::at_end() const noexcept {
  if (error()) { return error(); }
  return first.at_end();
}


simdjson2_inline int32_t simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document>::current_depth() const noexcept {
  if (error()) { return error(); }
  return first.current_depth();
}

simdjson2_inline simdjson2_result<std::string_view> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document>::raw_json_token() noexcept {
  if (error()) { return error(); }
  return first.raw_json_token();
}

simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::value> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document>::at_pointer(std::string_view json_pointer) noexcept {
  if (error()) { return error(); }
  return first.at_pointer(json_pointer);
}

simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::value> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document>::at_path(std::string_view json_path) noexcept {
  if (error()) { return error(); }
  return first.at_path(json_path);
}

} // namespace simdjson2


namespace simdjson2 {
namespace SIMDJSON2_IMPLEMENTATION {
namespace ondemand {

simdjson2_inline document_reference::document_reference() noexcept : doc{nullptr} {}
simdjson2_inline document_reference::document_reference(document &d) noexcept : doc(&d) {}
simdjson2_inline void document_reference::rewind() noexcept { doc->rewind(); }
simdjson2_inline simdjson2_result<array> document_reference::get_array() & noexcept { return doc->get_array(); }
simdjson2_inline simdjson2_result<object> document_reference::get_object() & noexcept { return doc->get_object(); }
/**
 * The document_reference instances are used primarily/solely for streams of JSON
 * documents.
 * We decided that calling 'get_double()' on the JSON document '1.233 blabla' should
 * give an error, so we check for trailing content.
 *
 * However, for streams of JSON documents, we want to be able to start from
 * "321" "321" "321"
 * and parse it successfully as a stream of JSON documents, calling get_uint64_in_string()
 * successfully each time.
 *
 * To achieve this result, we pass a 'false' to a get_root_value_iterator() method:
 * this indicates that we allow trailing content.
 */
simdjson2_inline simdjson2_result<uint64_t> document_reference::get_uint64() noexcept { return doc->get_root_value_iterator().get_root_uint64(false); }
simdjson2_inline simdjson2_result<uint64_t> document_reference::get_uint64_in_string() noexcept { return doc->get_root_value_iterator().get_root_uint64_in_string(false); }
simdjson2_inline simdjson2_result<int64_t> document_reference::get_int64() noexcept { return doc->get_root_value_iterator().get_root_int64(false); }
simdjson2_inline simdjson2_result<int64_t> document_reference::get_int64_in_string() noexcept { return doc->get_root_value_iterator().get_root_int64_in_string(false); }
simdjson2_inline simdjson2_result<double> document_reference::get_double() noexcept { return doc->get_root_value_iterator().get_root_double(false); }
simdjson2_inline simdjson2_result<double> document_reference::get_double_in_string() noexcept { return doc->get_root_value_iterator().get_root_double(false); }
simdjson2_inline simdjson2_result<std::string_view> document_reference::get_string(bool allow_replacement) noexcept { return doc->get_root_value_iterator().get_root_string(false, allow_replacement); }
template <typename string_type>
simdjson2_inline error_code document_reference::get_string(string_type& receiver, bool allow_replacement) noexcept { return doc->get_root_value_iterator().get_root_string(receiver, false, allow_replacement); }
simdjson2_inline simdjson2_result<std::string_view> document_reference::get_wobbly_string() noexcept { return doc->get_root_value_iterator().get_root_wobbly_string(false); }
simdjson2_inline simdjson2_result<raw_json_string> document_reference::get_raw_json_string() noexcept { return doc->get_root_value_iterator().get_root_raw_json_string(false); }
simdjson2_inline simdjson2_result<bool> document_reference::get_bool() noexcept { return doc->get_root_value_iterator().get_root_bool(false); }
simdjson2_inline simdjson2_result<value> document_reference::get_value() noexcept { return doc->get_value(); }
simdjson2_inline simdjson2_result<bool> document_reference::is_null() noexcept { return doc->get_root_value_iterator().is_root_null(false); }
template<> simdjson2_inline simdjson2_result<array> document_reference::get() & noexcept { return get_array(); }
template<> simdjson2_inline simdjson2_result<object> document_reference::get() & noexcept { return get_object(); }
template<> simdjson2_inline simdjson2_result<raw_json_string> document_reference::get() & noexcept { return get_raw_json_string(); }
template<> simdjson2_inline simdjson2_result<std::string_view> document_reference::get() & noexcept { return get_string(false); }
template<> simdjson2_inline simdjson2_result<double> document_reference::get() & noexcept { return get_double(); }
template<> simdjson2_inline simdjson2_result<uint64_t> document_reference::get() & noexcept { return get_uint64(); }
template<> simdjson2_inline simdjson2_result<int64_t> document_reference::get() & noexcept { return get_int64(); }
template<> simdjson2_inline simdjson2_result<bool> document_reference::get() & noexcept { return get_bool(); }
template<> simdjson2_inline simdjson2_result<value> document_reference::get() & noexcept { return get_value(); }
#if SIMDJSON2_EXCEPTIONS
template <class T>
simdjson2_inline document_reference::operator T() noexcept(false) { return get<T>(); }
simdjson2_inline document_reference::operator array() & noexcept(false) { return array(*doc); }
simdjson2_inline document_reference::operator object() & noexcept(false) { return object(*doc); }
simdjson2_inline document_reference::operator uint64_t() noexcept(false) { return get_uint64(); }
simdjson2_inline document_reference::operator int64_t() noexcept(false) { return get_int64(); }
simdjson2_inline document_reference::operator double() noexcept(false) { return get_double(); }
simdjson2_inline document_reference::operator std::string_view() noexcept(false) { return std::string_view(*doc); }
simdjson2_inline document_reference::operator raw_json_string() noexcept(false) { return get_raw_json_string(); }
simdjson2_inline document_reference::operator bool() noexcept(false) { return get_bool(); }
simdjson2_inline document_reference::operator value() noexcept(false) { return value(*doc); }
#endif
simdjson2_inline simdjson2_result<size_t> document_reference::count_elements() & noexcept { return doc->count_elements(); }
simdjson2_inline simdjson2_result<size_t> document_reference::count_fields() & noexcept { return doc->count_fields(); }
simdjson2_inline simdjson2_result<value> document_reference::at(size_t index) & noexcept { return doc->at(index); }
simdjson2_inline simdjson2_result<array_iterator> document_reference::begin() & noexcept { return doc->begin(); }
simdjson2_inline simdjson2_result<array_iterator> document_reference::end() & noexcept { return doc->end(); }
simdjson2_inline simdjson2_result<value> document_reference::find_field(std::string_view key) & noexcept { return doc->find_field(key); }
simdjson2_inline simdjson2_result<value> document_reference::find_field(const char *key) & noexcept { return doc->find_field(key); }
simdjson2_inline simdjson2_result<value> document_reference::operator[](std::string_view key) & noexcept { return (*doc)[key]; }
simdjson2_inline simdjson2_result<value> document_reference::operator[](const char *key) & noexcept { return (*doc)[key]; }
simdjson2_inline simdjson2_result<value> document_reference::find_field_unordered(std::string_view key) & noexcept { return doc->find_field_unordered(key); }
simdjson2_inline simdjson2_result<value> document_reference::find_field_unordered(const char *key) & noexcept { return doc->find_field_unordered(key); }
simdjson2_inline simdjson2_result<json_type> document_reference::type() noexcept { return doc->type(); }
simdjson2_inline simdjson2_result<bool> document_reference::is_scalar() noexcept { return doc->is_scalar(); }
simdjson2_inline simdjson2_result<bool> document_reference::is_string() noexcept { return doc->is_string(); }
simdjson2_inline simdjson2_result<const char *> document_reference::current_location() noexcept { return doc->current_location(); }
simdjson2_inline int32_t document_reference::current_depth() const noexcept { return doc->current_depth(); }
simdjson2_inline bool document_reference::is_negative() noexcept { return doc->is_negative(); }
simdjson2_inline simdjson2_result<bool> document_reference::is_integer() noexcept { return doc->get_root_value_iterator().is_root_integer(false); }
simdjson2_inline simdjson2_result<number_type> document_reference::get_number_type() noexcept { return doc->get_root_value_iterator().get_root_number_type(false); }
simdjson2_inline simdjson2_result<number> document_reference::get_number() noexcept { return doc->get_root_value_iterator().get_root_number(false); }
simdjson2_inline simdjson2_result<std::string_view> document_reference::raw_json_token() noexcept { return doc->raw_json_token(); }
simdjson2_inline simdjson2_result<value> document_reference::at_pointer(std::string_view json_pointer) noexcept { return doc->at_pointer(json_pointer); }
simdjson2_inline simdjson2_result<value> document_reference::at_path(std::string_view json_path) noexcept { return doc->at_path(json_path); }
simdjson2_inline simdjson2_result<std::string_view> document_reference::raw_json() noexcept { return doc->raw_json();}
simdjson2_inline document_reference::operator document&() const noexcept { return *doc; }

} // namespace ondemand
} // namespace SIMDJSON2_IMPLEMENTATION
} // namespace simdjson2



namespace simdjson2 {
simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document_reference>::simdjson2_result(SIMDJSON2_IMPLEMENTATION::ondemand::document_reference value, error_code error)
  noexcept : implementation_simdjson2_result_base<SIMDJSON2_IMPLEMENTATION::ondemand::document_reference>(std::forward<SIMDJSON2_IMPLEMENTATION::ondemand::document_reference>(value), error) {}


simdjson2_inline simdjson2_result<size_t> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document_reference>::count_elements() & noexcept {
  if (error()) { return error(); }
  return first.count_elements();
}
simdjson2_inline simdjson2_result<size_t> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document_reference>::count_fields() & noexcept {
  if (error()) { return error(); }
  return first.count_fields();
}
simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::value> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document_reference>::at(size_t index) & noexcept {
  if (error()) { return error(); }
  return first.at(index);
}
simdjson2_inline error_code simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document_reference>::rewind() noexcept {
  if (error()) { return error(); }
  first.rewind();
  return SUCCESS;
}
simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::array_iterator> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document_reference>::begin() & noexcept {
  if (error()) { return error(); }
  return first.begin();
}
simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::array_iterator> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document_reference>::end() & noexcept {
  return {};
}
simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::value> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document_reference>::find_field_unordered(std::string_view key) & noexcept {
  if (error()) { return error(); }
  return first.find_field_unordered(key);
}
simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::value> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document_reference>::find_field_unordered(const char *key) & noexcept {
  if (error()) { return error(); }
  return first.find_field_unordered(key);
}
simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::value> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document_reference>::operator[](std::string_view key) & noexcept {
  if (error()) { return error(); }
  return first[key];
}
simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::value> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document_reference>::operator[](const char *key) & noexcept {
  if (error()) { return error(); }
  return first[key];
}
simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::value> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document_reference>::find_field(std::string_view key) & noexcept {
  if (error()) { return error(); }
  return first.find_field(key);
}
simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::value> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document_reference>::find_field(const char *key) & noexcept {
  if (error()) { return error(); }
  return first.find_field(key);
}
simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::array> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document_reference>::get_array() & noexcept {
  if (error()) { return error(); }
  return first.get_array();
}
simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::object> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document_reference>::get_object() & noexcept {
  if (error()) { return error(); }
  return first.get_object();
}
simdjson2_inline simdjson2_result<uint64_t> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document_reference>::get_uint64() noexcept {
  if (error()) { return error(); }
  return first.get_uint64();
}
simdjson2_inline simdjson2_result<uint64_t> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document_reference>::get_uint64_in_string() noexcept {
  if (error()) { return error(); }
  return first.get_uint64_in_string();
}
simdjson2_inline simdjson2_result<int64_t> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document_reference>::get_int64() noexcept {
  if (error()) { return error(); }
  return first.get_int64();
}
simdjson2_inline simdjson2_result<int64_t> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document_reference>::get_int64_in_string() noexcept {
  if (error()) { return error(); }
  return first.get_int64_in_string();
}
simdjson2_inline simdjson2_result<double> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document_reference>::get_double() noexcept {
  if (error()) { return error(); }
  return first.get_double();
}
simdjson2_inline simdjson2_result<double> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document_reference>::get_double_in_string() noexcept {
  if (error()) { return error(); }
  return first.get_double_in_string();
}
simdjson2_inline simdjson2_result<std::string_view> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document_reference>::get_string(bool allow_replacement) noexcept {
  if (error()) { return error(); }
  return first.get_string(allow_replacement);
}
template <typename string_type>
simdjson2_inline error_code simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document_reference>::get_string(string_type& receiver, bool allow_replacement) noexcept {
  if (error()) { return error(); }
  return first.get_string(receiver, allow_replacement);
}
simdjson2_inline simdjson2_result<std::string_view> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document_reference>::get_wobbly_string() noexcept {
  if (error()) { return error(); }
  return first.get_wobbly_string();
}
simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::raw_json_string> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document_reference>::get_raw_json_string() noexcept {
  if (error()) { return error(); }
  return first.get_raw_json_string();
}
simdjson2_inline simdjson2_result<bool> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document_reference>::get_bool() noexcept {
  if (error()) { return error(); }
  return first.get_bool();
}
simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::value> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document_reference>::get_value() noexcept {
  if (error()) { return error(); }
  return first.get_value();
}
simdjson2_inline simdjson2_result<bool> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document_reference>::is_null() noexcept {
  if (error()) { return error(); }
  return first.is_null();
}
template<typename T>
simdjson2_inline simdjson2_result<T> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document_reference>::get() & noexcept {
  if (error()) { return error(); }
  return first.get<T>();
}
template<typename T>
simdjson2_inline simdjson2_result<T> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document_reference>::get() && noexcept {
  if (error()) { return error(); }
  return std::forward<SIMDJSON2_IMPLEMENTATION::ondemand::document_reference>(first).get<T>();
}
template <class T>
simdjson2_inline error_code simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document_reference>::get(T &out) & noexcept {
  if (error()) { return error(); }
  return first.get<T>(out);
}
template <class T>
simdjson2_inline error_code simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document_reference>::get(T &out) && noexcept {
  if (error()) { return error(); }
  return std::forward<SIMDJSON2_IMPLEMENTATION::ondemand::document_reference>(first).get<T>(out);
}
simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::json_type> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document_reference>::type() noexcept {
  if (error()) { return error(); }
  return first.type();
}
simdjson2_inline simdjson2_result<bool> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document_reference>::is_scalar() noexcept {
  if (error()) { return error(); }
  return first.is_scalar();
}
simdjson2_inline simdjson2_result<bool> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document_reference>::is_string() noexcept {
  if (error()) { return error(); }
  return first.is_string();
}
template <>
simdjson2_inline error_code simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document_reference>::get(SIMDJSON2_IMPLEMENTATION::ondemand::document_reference &out) & noexcept {
  if (error()) { return error(); }
  out = first;
  return SUCCESS;
}
template <>
simdjson2_inline error_code simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document_reference>::get(SIMDJSON2_IMPLEMENTATION::ondemand::document_reference &out) && noexcept {
  if (error()) { return error(); }
  out = first;
  return SUCCESS;
}
simdjson2_inline simdjson2_result<bool> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document_reference>::is_negative() noexcept {
  if (error()) { return error(); }
  return first.is_negative();
}
simdjson2_inline simdjson2_result<bool> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document_reference>::is_integer() noexcept {
  if (error()) { return error(); }
  return first.is_integer();
}
simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::number_type> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document_reference>::get_number_type() noexcept {
  if (error()) { return error(); }
  return first.get_number_type();
}
simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::number> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document_reference>::get_number() noexcept {
  if (error()) { return error(); }
  return first.get_number();
}
#if SIMDJSON2_EXCEPTIONS
template <class T>
simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document_reference>::operator T() noexcept(false) {
  static_assert(std::is_same<T, SIMDJSON2_IMPLEMENTATION::ondemand::document_reference>::value == false, "You should not call get<T> when T is a document");
  static_assert(std::is_same<T, SIMDJSON2_IMPLEMENTATION::ondemand::document>::value == false, "You should not call get<T> when T is a document");
  if (error()) { throw simdjson2_error(error()); }
  return first.get<T>();
}
simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document_reference>::operator SIMDJSON2_IMPLEMENTATION::ondemand::array() & noexcept(false) {
  if (error()) { throw simdjson2_error(error()); }
  return first;
}
simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document_reference>::operator SIMDJSON2_IMPLEMENTATION::ondemand::object() & noexcept(false) {
  if (error()) { throw simdjson2_error(error()); }
  return first;
}
simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document_reference>::operator uint64_t() noexcept(false) {
  if (error()) { throw simdjson2_error(error()); }
  return first;
}
simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document_reference>::operator int64_t() noexcept(false) {
  if (error()) { throw simdjson2_error(error()); }
  return first;
}
simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document_reference>::operator double() noexcept(false) {
  if (error()) { throw simdjson2_error(error()); }
  return first;
}
simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document_reference>::operator std::string_view() noexcept(false) {
  if (error()) { throw simdjson2_error(error()); }
  return first;
}
simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document_reference>::operator SIMDJSON2_IMPLEMENTATION::ondemand::raw_json_string() noexcept(false) {
  if (error()) { throw simdjson2_error(error()); }
  return first;
}
simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document_reference>::operator bool() noexcept(false) {
  if (error()) { throw simdjson2_error(error()); }
  return first;
}
simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document_reference>::operator SIMDJSON2_IMPLEMENTATION::ondemand::value() noexcept(false) {
  if (error()) { throw simdjson2_error(error()); }
  return first;
}
#endif

simdjson2_inline simdjson2_result<const char *> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document_reference>::current_location() noexcept {
  if (error()) { return error(); }
  return first.current_location();
}

simdjson2_inline simdjson2_result<std::string_view> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document_reference>::raw_json_token() noexcept {
  if (error()) { return error(); }
  return first.raw_json_token();
}

simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::value> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document_reference>::at_pointer(std::string_view json_pointer) noexcept {
  if (error()) { return error(); }
  return first.at_pointer(json_pointer);
}

simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::value> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document_reference>::at_path(std::string_view json_path) noexcept {
  if (error()) {
      return error();
  }
  return first.at_path(json_path);
}

} // namespace simdjson2

#endif // SIMDJSON2_GENERIC_ONDEMAND_DOCUMENT_INL_H
