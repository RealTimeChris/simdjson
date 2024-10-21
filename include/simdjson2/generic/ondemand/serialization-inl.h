#ifndef SIMDJSON2_GENERIC_ONDEMAND_SERIALIZATION_INL_H

#ifndef SIMDJSON2_CONDITIONAL_INCLUDE
#define SIMDJSON2_GENERIC_ONDEMAND_SERIALIZATION_INL_H
#include "simdjson2/generic/ondemand/base.h"
#include "simdjson2/generic/ondemand/array.h"
#include "simdjson2/generic/ondemand/document-inl.h"
#include "simdjson2/generic/ondemand/json_type.h"
#include "simdjson2/generic/ondemand/object.h"
#include "simdjson2/generic/ondemand/serialization.h"
#include "simdjson2/generic/ondemand/value.h"
#endif // SIMDJSON2_CONDITIONAL_INCLUDE

namespace simdjson2 {

inline std::string_view trim(const std::string_view str) noexcept {
  // We can almost surely do better by rolling our own find_first_not_of function.
  size_t first = str.find_first_not_of(" \t\n\r");
  // If we have the empty string (just white space), then no trimming is possible, and
  // we return the empty string_view.
  if (std::string_view::npos == first) { return std::string_view(); }
  size_t last = str.find_last_not_of(" \t\n\r");
  return str.substr(first, (last - first + 1));
}


inline simdjson2_result<std::string_view> to_json_string(SIMDJSON2_IMPLEMENTATION::ondemand::document& x) noexcept {
  std::string_view v;
  auto error = x.raw_json().get(v);
  if(error) {return error; }
  return trim(v);
}

inline simdjson2_result<std::string_view> to_json_string(SIMDJSON2_IMPLEMENTATION::ondemand::document_reference& x) noexcept {
  std::string_view v;
  auto error = x.raw_json().get(v);
  if(error) {return error; }
  return trim(v);
}

inline simdjson2_result<std::string_view> to_json_string(SIMDJSON2_IMPLEMENTATION::ondemand::value& x) noexcept {
  /**
   * If we somehow receive a value that has already been consumed,
   * then the following code could be in trouble. E.g., we create
   * an array as needed, but if an array was already created, then
   * it could be bad.
   */
  using namespace SIMDJSON2_IMPLEMENTATION::ondemand;
  SIMDJSON2_IMPLEMENTATION::ondemand::json_type t;
  auto error = x.type().get(t);
  if(error != SUCCESS) { return error; }
  switch (t)
  {
    case json_type::array:
    {
      SIMDJSON2_IMPLEMENTATION::ondemand::array array;
      error = x.get_array().get(array);
      if(error) { return error; }
      return to_json_string(array);
    }
    case json_type::object:
    {
      SIMDJSON2_IMPLEMENTATION::ondemand::object object;
      error = x.get_object().get(object);
      if(error) { return error; }
      return to_json_string(object);
    }
    default:
      return trim(x.raw_json_token());
  }
}

inline simdjson2_result<std::string_view> to_json_string(SIMDJSON2_IMPLEMENTATION::ondemand::object& x) noexcept {
  std::string_view v;
  auto error = x.raw_json().get(v);
  if(error) {return error; }
  return trim(v);
}

inline simdjson2_result<std::string_view> to_json_string(SIMDJSON2_IMPLEMENTATION::ondemand::array& x) noexcept {
  std::string_view v;
  auto error = x.raw_json().get(v);
  if(error) {return error; }
  return trim(v);
}

inline simdjson2_result<std::string_view> to_json_string(simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document> x) {
  if (x.error()) { return x.error(); }
  return to_json_string(x.value_unsafe());
}

inline simdjson2_result<std::string_view> to_json_string(simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document_reference> x) {
  if (x.error()) { return x.error(); }
  return to_json_string(x.value_unsafe());
}

inline simdjson2_result<std::string_view> to_json_string(simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::value> x) {
  if (x.error()) { return x.error(); }
  return to_json_string(x.value_unsafe());
}

inline simdjson2_result<std::string_view> to_json_string(simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::object> x) {
  if (x.error()) { return x.error(); }
  return to_json_string(x.value_unsafe());
}

inline simdjson2_result<std::string_view> to_json_string(simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::array> x) {
  if (x.error()) { return x.error(); }
  return to_json_string(x.value_unsafe());
}
} // namespace simdjson2

namespace simdjson2 { namespace SIMDJSON2_IMPLEMENTATION { namespace ondemand {

#if SIMDJSON2_EXCEPTIONS
inline std::ostream& operator<<(std::ostream& out, simdjson2::SIMDJSON2_IMPLEMENTATION::ondemand::value x) {
  std::string_view v;
  auto error = simdjson2::to_json_string(x).get(v);
  if(error == simdjson2::SUCCESS) {
    return (out << v);
  } else {
    throw simdjson2::simdjson2_error(error);
  }
}
inline std::ostream& operator<<(std::ostream& out, simdjson2::simdjson2_result<simdjson2::SIMDJSON2_IMPLEMENTATION::ondemand::value> x) {
  if (x.error()) { throw simdjson2::simdjson2_error(x.error()); }
  return (out << x.value());
}
#else
inline std::ostream& operator<<(std::ostream& out, simdjson2::SIMDJSON2_IMPLEMENTATION::ondemand::value x) {
  std::string_view v;
  auto error = simdjson2::to_json_string(x).get(v);
  if(error == simdjson2::SUCCESS) {
    return (out << v);
  } else {
    return (out << error);
  }
}
#endif

#if SIMDJSON2_EXCEPTIONS
inline std::ostream& operator<<(std::ostream& out, simdjson2::SIMDJSON2_IMPLEMENTATION::ondemand::array value) {
  std::string_view v;
  auto error = simdjson2::to_json_string(value).get(v);
  if(error == simdjson2::SUCCESS) {
    return (out << v);
  } else {
    throw simdjson2::simdjson2_error(error);
  }
}
inline std::ostream& operator<<(std::ostream& out, simdjson2::simdjson2_result<simdjson2::SIMDJSON2_IMPLEMENTATION::ondemand::array> x) {
  if (x.error()) { throw simdjson2::simdjson2_error(x.error()); }
  return (out << x.value());
}
#else
inline std::ostream& operator<<(std::ostream& out, simdjson2::SIMDJSON2_IMPLEMENTATION::ondemand::array value) {
  std::string_view v;
  auto error = simdjson2::to_json_string(value).get(v);
  if(error == simdjson2::SUCCESS) {
    return (out << v);
  } else {
    return (out << error);
  }
}
#endif

#if SIMDJSON2_EXCEPTIONS
inline std::ostream& operator<<(std::ostream& out, simdjson2::SIMDJSON2_IMPLEMENTATION::ondemand::document& value)  {
  std::string_view v;
  auto error = simdjson2::to_json_string(value).get(v);
  if(error == simdjson2::SUCCESS) {
    return (out << v);
  } else {
    throw simdjson2::simdjson2_error(error);
  }
}
inline std::ostream& operator<<(std::ostream& out, simdjson2::SIMDJSON2_IMPLEMENTATION::ondemand::document_reference& value)  {
  std::string_view v;
  auto error = simdjson2::to_json_string(value).get(v);
  if(error == simdjson2::SUCCESS) {
    return (out << v);
  } else {
    throw simdjson2::simdjson2_error(error);
  }
}
inline std::ostream& operator<<(std::ostream& out, simdjson2::simdjson2_result<simdjson2::SIMDJSON2_IMPLEMENTATION::ondemand::document>&& x) {
  if (x.error()) { throw simdjson2::simdjson2_error(x.error()); }
  return (out << x.value());
}
inline std::ostream& operator<<(std::ostream& out, simdjson2::simdjson2_result<simdjson2::SIMDJSON2_IMPLEMENTATION::ondemand::document_reference>&& x) {
  if (x.error()) { throw simdjson2::simdjson2_error(x.error()); }
  return (out << x.value());
}
#else
inline std::ostream& operator<<(std::ostream& out, simdjson2::SIMDJSON2_IMPLEMENTATION::ondemand::document& value)  {
  std::string_view v;
  auto error = simdjson2::to_json_string(value).get(v);
  if(error == simdjson2::SUCCESS) {
    return (out << v);
  } else {
    return (out << error);
  }
}
#endif

#if SIMDJSON2_EXCEPTIONS
inline std::ostream& operator<<(std::ostream& out, simdjson2::SIMDJSON2_IMPLEMENTATION::ondemand::object value) {
  std::string_view v;
  auto error = simdjson2::to_json_string(value).get(v);
  if(error == simdjson2::SUCCESS) {
    return (out << v);
  } else {
    throw simdjson2::simdjson2_error(error);
  }
}
inline std::ostream& operator<<(std::ostream& out,  simdjson2::simdjson2_result<simdjson2::SIMDJSON2_IMPLEMENTATION::ondemand::object> x) {
  if (x.error()) { throw  simdjson2::simdjson2_error(x.error()); }
  return (out << x.value());
}
#else
inline std::ostream& operator<<(std::ostream& out, simdjson2::SIMDJSON2_IMPLEMENTATION::ondemand::object value) {
  std::string_view v;
  auto error = simdjson2::to_json_string(value).get(v);
  if(error == simdjson2::SUCCESS) {
    return (out << v);
  } else {
    return (out << error);
  }
}
#endif
}}} // namespace simdjson2::SIMDJSON2_IMPLEMENTATION::ondemand

#endif // SIMDJSON2_GENERIC_ONDEMAND_SERIALIZATION_INL_H