#ifndef SIMDJSON2_GENERIC_ONDEMAND_JSON_TYPE_INL_H

#ifndef SIMDJSON2_CONDITIONAL_INCLUDE
#define SIMDJSON2_GENERIC_ONDEMAND_JSON_TYPE_INL_H
#include "simdjson2/generic/ondemand/base.h"
#include "simdjson2/generic/ondemand/json_type.h"
#include "simdjson2/generic/implementation_simdjson2_result_base-inl.h"
#endif // SIMDJSON2_CONDITIONAL_INCLUDE

namespace simdjson2 {
namespace SIMDJSON2_IMPLEMENTATION {
namespace ondemand {

inline std::ostream& operator<<(std::ostream& out, json_type type) noexcept {
    switch (type) {
        case json_type::array: out << "array"; break;
        case json_type::object: out << "object"; break;
        case json_type::number: out << "number"; break;
        case json_type::string: out << "string"; break;
        case json_type::boolean: out << "boolean"; break;
        case json_type::null: out << "null"; break;
        default: SIMDJSON2_UNREACHABLE();
    }
    return out;
}

#if SIMDJSON2_EXCEPTIONS
inline std::ostream& operator<<(std::ostream& out, simdjson2_result<json_type> &type) noexcept(false) {
    return out << type.value();
}
#endif



simdjson2_inline number_type number::get_number_type() const noexcept {
  return type;
}

simdjson2_inline bool number::is_uint64() const noexcept {
  return get_number_type() == number_type::unsigned_integer;
}

simdjson2_inline uint64_t number::get_uint64() const noexcept {
  return payload.unsigned_integer;
}

simdjson2_inline number::operator uint64_t() const noexcept {
  return get_uint64();
}

simdjson2_inline bool number::is_int64() const noexcept {
  return get_number_type() == number_type::signed_integer;
}

simdjson2_inline int64_t number::get_int64() const noexcept {
  return payload.signed_integer;
}

simdjson2_inline number::operator int64_t() const noexcept {
  return get_int64();
}

simdjson2_inline bool number::is_double() const noexcept {
    return get_number_type() == number_type::floating_point_number;
}

simdjson2_inline double number::get_double() const noexcept {
  return payload.floating_point_number;
}

simdjson2_inline number::operator double() const noexcept {
  return get_double();
}

simdjson2_inline double number::as_double() const noexcept {
  if(is_double()) {
    return payload.floating_point_number;
  }
  if(is_int64()) {
    return double(payload.signed_integer);
  }
  return double(payload.unsigned_integer);
}

simdjson2_inline void number::append_s64(int64_t value) noexcept {
  payload.signed_integer = value;
  type = number_type::signed_integer;
}

simdjson2_inline void number::append_u64(uint64_t value) noexcept {
  payload.unsigned_integer = value;
  type = number_type::unsigned_integer;
}

simdjson2_inline void number::append_double(double value) noexcept {
  payload.floating_point_number = value;
  type = number_type::floating_point_number;
}

simdjson2_inline void number::skip_double() noexcept {
  type = number_type::floating_point_number;
}

} // namespace ondemand
} // namespace SIMDJSON2_IMPLEMENTATION
} // namespace simdjson2

namespace simdjson2 {

simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::json_type>::simdjson2_result(SIMDJSON2_IMPLEMENTATION::ondemand::json_type &&value) noexcept
    : implementation_simdjson2_result_base<SIMDJSON2_IMPLEMENTATION::ondemand::json_type>(std::forward<SIMDJSON2_IMPLEMENTATION::ondemand::json_type>(value)) {}
simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::json_type>::simdjson2_result(error_code error) noexcept
    : implementation_simdjson2_result_base<SIMDJSON2_IMPLEMENTATION::ondemand::json_type>(error) {}

} // namespace simdjson2

#endif // SIMDJSON2_GENERIC_ONDEMAND_JSON_TYPE_INL_H