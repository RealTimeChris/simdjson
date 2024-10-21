#ifndef SIMDJSON2_GENERIC_IMPLEMENTATION_SIMDJSON2_RESULT_BASE_INL_H

#ifndef SIMDJSON2_CONDITIONAL_INCLUDE
#define SIMDJSON2_GENERIC_IMPLEMENTATION_SIMDJSON2_RESULT_BASE_INL_H
#include "simdjson2/generic/base.h"
#include "simdjson2/generic/implementation_simdjson2_result_base.h"
#endif // SIMDJSON2_CONDITIONAL_INCLUDE

namespace simdjson2 {
namespace SIMDJSON2_IMPLEMENTATION {

//
// internal::implementation_simdjson2_result_base<T> inline implementation
//

template<typename T>
simdjson2_really_inline void implementation_simdjson2_result_base<T>::tie(T &value, error_code &error) && noexcept {
  error = this->second;
  if (!error) {
    value = std::forward<implementation_simdjson2_result_base<T>>(*this).first;
  }
}

template<typename T>
simdjson2_warn_unused simdjson2_really_inline error_code implementation_simdjson2_result_base<T>::get(T &value) && noexcept {
  error_code error;
  std::forward<implementation_simdjson2_result_base<T>>(*this).tie(value, error);
  return error;
}

template<typename T>
simdjson2_really_inline error_code implementation_simdjson2_result_base<T>::error() const noexcept {
  return this->second;
}

#if SIMDJSON2_EXCEPTIONS

template<typename T>
simdjson2_really_inline T& implementation_simdjson2_result_base<T>::value() & noexcept(false) {
  if (error()) { throw simdjson2_error(error()); }
  return this->first;
}

template<typename T>
simdjson2_really_inline T&& implementation_simdjson2_result_base<T>::value() && noexcept(false) {
  return std::forward<implementation_simdjson2_result_base<T>>(*this).take_value();
}

template<typename T>
simdjson2_really_inline T&& implementation_simdjson2_result_base<T>::take_value() && noexcept(false) {
  if (error()) { throw simdjson2_error(error()); }
  return std::forward<T>(this->first);
}

template<typename T>
simdjson2_really_inline implementation_simdjson2_result_base<T>::operator T&&() && noexcept(false) {
  return std::forward<implementation_simdjson2_result_base<T>>(*this).take_value();
}

#endif // SIMDJSON2_EXCEPTIONS

template<typename T>
simdjson2_really_inline const T& implementation_simdjson2_result_base<T>::value_unsafe() const& noexcept {
  return this->first;
}

template<typename T>
simdjson2_really_inline T& implementation_simdjson2_result_base<T>::value_unsafe() & noexcept {
  return this->first;
}

template<typename T>
simdjson2_really_inline T&& implementation_simdjson2_result_base<T>::value_unsafe() && noexcept {
  return std::forward<T>(this->first);
}

template<typename T>
simdjson2_really_inline implementation_simdjson2_result_base<T>::implementation_simdjson2_result_base(T &&value, error_code error) noexcept
    : first{std::forward<T>(value)}, second{error} {}
template<typename T>
simdjson2_really_inline implementation_simdjson2_result_base<T>::implementation_simdjson2_result_base(error_code error) noexcept
    : implementation_simdjson2_result_base(T{}, error) {}
template<typename T>
simdjson2_really_inline implementation_simdjson2_result_base<T>::implementation_simdjson2_result_base(T &&value) noexcept
    : implementation_simdjson2_result_base(std::forward<T>(value), SUCCESS) {}

} // namespace SIMDJSON2_IMPLEMENTATION
} // namespace simdjson2

#endif // SIMDJSON2_GENERIC_IMPLEMENTATION_SIMDJSON2_RESULT_BASE_INL_H
