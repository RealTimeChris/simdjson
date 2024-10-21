#ifndef SIMDJSON2_GENERIC_ONDEMAND_FIELD_INL_H

#ifndef SIMDJSON2_CONDITIONAL_INCLUDE
#define SIMDJSON2_GENERIC_ONDEMAND_FIELD_INL_H
#include "simdjson2/generic/ondemand/base.h"
#include "simdjson2/generic/ondemand/field.h"
#include "simdjson2/generic/ondemand/value-inl.h"
#include "simdjson2/generic/ondemand/value_iterator-inl.h"
#endif // SIMDJSON2_CONDITIONAL_INCLUDE

namespace simdjson2 {
namespace SIMDJSON2_IMPLEMENTATION {
namespace ondemand {

// clang 6 does not think the default constructor can be noexcept, so we make it explicit
simdjson2_inline field::field() noexcept : std::pair<raw_json_string, ondemand::value>() {}

simdjson2_inline field::field(raw_json_string key, ondemand::value &&value) noexcept
  : std::pair<raw_json_string, ondemand::value>(key, std::forward<ondemand::value>(value))
{
}

simdjson2_inline simdjson2_result<field> field::start(value_iterator &parent_iter) noexcept {
  raw_json_string key;
  SIMDJSON2_TRY( parent_iter.field_key().get(key) );
  SIMDJSON2_TRY( parent_iter.field_value() );
  return field::start(parent_iter, key);
}

simdjson2_inline simdjson2_result<field> field::start(const value_iterator &parent_iter, raw_json_string key) noexcept {
    return field(key, parent_iter.child());
}

simdjson2_inline simdjson2_warn_unused simdjson2_result<std::string_view> field::unescaped_key(bool allow_replacement) noexcept {
  SIMDJSON2_ASSUME(first.buf != nullptr); // We would like to call .alive() but Visual Studio won't let us.
  simdjson2_result<std::string_view> answer = first.unescape(second.iter.json_iter(), allow_replacement);
  first.consume();
  return answer;
}

template <typename string_type>
simdjson2_inline simdjson2_warn_unused error_code field::unescaped_key(string_type& receiver, bool allow_replacement) noexcept {
  std::string_view key;
  SIMDJSON2_TRY( unescaped_key(allow_replacement).get(key) );
  receiver = key;
  return SUCCESS;
}

simdjson2_inline raw_json_string field::key() const noexcept {
  SIMDJSON2_ASSUME(first.buf != nullptr); // We would like to call .alive() by Visual Studio won't let us.
  return first;
}


simdjson2_inline std::string_view field::key_raw_json_token() const noexcept {
  SIMDJSON2_ASSUME(first.buf != nullptr); // We would like to call .alive() by Visual Studio won't let us.
  return std::string_view(reinterpret_cast<const char*>(first.buf-1), second.iter._json_iter->token.peek(-1) - first.buf + 1);
}

simdjson2_inline std::string_view field::escaped_key() const noexcept {
  SIMDJSON2_ASSUME(first.buf != nullptr); // We would like to call .alive() by Visual Studio won't let us.
  auto end_quote = second.iter._json_iter->token.peek(-1);
  while(*end_quote != '"') end_quote--;
  return std::string_view(reinterpret_cast<const char*>(first.buf), end_quote - first.buf);
}

simdjson2_inline value &field::value() & noexcept {
  return second;
}

simdjson2_inline value field::value() && noexcept {
  return std::forward<field>(*this).second;
}

} // namespace ondemand
} // namespace SIMDJSON2_IMPLEMENTATION
} // namespace simdjson2

namespace simdjson2 {

simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::field>::simdjson2_result(
  SIMDJSON2_IMPLEMENTATION::ondemand::field &&value
) noexcept :
    implementation_simdjson2_result_base<SIMDJSON2_IMPLEMENTATION::ondemand::field>(
      std::forward<SIMDJSON2_IMPLEMENTATION::ondemand::field>(value)
    )
{
}
simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::field>::simdjson2_result(
  error_code error
) noexcept :
    implementation_simdjson2_result_base<SIMDJSON2_IMPLEMENTATION::ondemand::field>(error)
{
}

simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::raw_json_string> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::field>::key() noexcept {
  if (error()) { return error(); }
  return first.key();
}

simdjson2_inline simdjson2_result<std::string_view> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::field>::key_raw_json_token() noexcept {
  if (error()) { return error(); }
  return first.key_raw_json_token();
}

simdjson2_inline simdjson2_result<std::string_view> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::field>::escaped_key() noexcept {
  if (error()) { return error(); }
  return first.escaped_key();
}

simdjson2_inline simdjson2_result<std::string_view> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::field>::unescaped_key(bool allow_replacement) noexcept {
  if (error()) { return error(); }
  return first.unescaped_key(allow_replacement);
}

template<typename string_type>
simdjson2_inline error_code simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::field>::unescaped_key(string_type &receiver, bool allow_replacement) noexcept {
  if (error()) { return error(); }
  return first.unescaped_key(receiver, allow_replacement);
}

simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::value> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::field>::value() noexcept {
  if (error()) { return error(); }
  return std::move(first.value());
}

} // namespace simdjson2

#endif // SIMDJSON2_GENERIC_ONDEMAND_FIELD_INL_H