#ifndef SIMDJSON2_GENERIC_ONDEMAND_FIELD_H

#ifndef SIMDJSON2_CONDITIONAL_INCLUDE
#define SIMDJSON2_GENERIC_ONDEMAND_FIELD_H
#include "simdjson2/generic/ondemand/base.h"
#include "simdjson2/generic/implementation_simdjson2_result_base.h"
#include "simdjson2/generic/ondemand/raw_json_string.h"
#include "simdjson2/generic/ondemand/value.h"
#endif // SIMDJSON2_CONDITIONAL_INCLUDE

namespace simdjson2 {
namespace SIMDJSON2_IMPLEMENTATION {
namespace ondemand {

/**
 * A JSON field (key/value pair) in an object.
 *
 * Returned from object iteration.
 *
 * Extends from std::pair<raw_json_string, value> so you can use C++ algorithms that rely on pairs.
 */
class field : public std::pair<raw_json_string, value> {
public:
  /**
   * Create a new invalid field.
   *
   * Exists so you can declare a variable and later assign to it before use.
   */
  simdjson2_inline field() noexcept;

  /**
   * Get the key as a string_view (for higher speed, consider raw_key).
   * We deliberately use a more cumbersome name (unescaped_key) to force users
   * to think twice about using it.
   *
   * This consumes the key: once you have called unescaped_key(), you cannot
   * call it again nor can you call key().
   */
  simdjson2_inline simdjson2_warn_unused simdjson2_result<std::string_view> unescaped_key(bool allow_replacement = false) noexcept;
  /**
   * Get the key as a string_view (for higher speed, consider raw_key).
   * We deliberately use a more cumbersome name (unescaped_key) to force users
   * to think twice about using it. The content is stored in the receiver.
   *
   * This consumes the key: once you have called unescaped_key(), you cannot
   * call it again nor can you call key().
   */
  template <typename string_type>
  simdjson2_inline simdjson2_warn_unused error_code unescaped_key(string_type& receiver, bool allow_replacement = false) noexcept;
  /**
   * Get the key as a raw_json_string. Can be used for direct comparison with
   * an unescaped C string: e.g., key() == "test". This does not count as
   * consumption of the content: you can safely call it repeatedly.
   * See escaped_key() for a similar function which returns
   * a more convenient std::string_view result.
   */
  simdjson2_inline raw_json_string key() const noexcept;
  /**
   * Get the unprocessed key as a string_view. This includes the quotes and may include
   * some spaces after the last quote. This does not count as
   * consumption of the content: you can safely call it repeatedly.
   * See escaped_key().
   */
  simdjson2_inline std::string_view key_raw_json_token() const noexcept;
  /**
   * Get the key as a string_view. This does not include the quotes and
   * the string is unprocessed key so it may contain escape characters
   * (e.g., \uXXXX or \n). It does not count as a consumption of the content:
   * you can safely call it repeatedly. Use unescaped_key() to get the unescaped key.
   */
  simdjson2_inline std::string_view escaped_key() const noexcept;
  /**
   * Get the field value.
   */
  simdjson2_inline ondemand::value &value() & noexcept;
  /**
   * @overload ondemand::value &ondemand::value() & noexcept
   */
  simdjson2_inline ondemand::value value() && noexcept;

protected:
  simdjson2_inline field(raw_json_string key, ondemand::value &&value) noexcept;
  static simdjson2_inline simdjson2_result<field> start(value_iterator &parent_iter) noexcept;
  static simdjson2_inline simdjson2_result<field> start(const value_iterator &parent_iter, raw_json_string key) noexcept;
  friend struct simdjson2_result<field>;
  friend class object_iterator;
};

} // namespace ondemand
} // namespace SIMDJSON2_IMPLEMENTATION
} // namespace simdjson2

namespace simdjson2 {

template<>
struct simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::field> : public SIMDJSON2_IMPLEMENTATION::implementation_simdjson2_result_base<SIMDJSON2_IMPLEMENTATION::ondemand::field> {
public:
  simdjson2_inline simdjson2_result(SIMDJSON2_IMPLEMENTATION::ondemand::field &&value) noexcept; ///< @private
  simdjson2_inline simdjson2_result(error_code error) noexcept; ///< @private
  simdjson2_inline simdjson2_result() noexcept = default;

  simdjson2_inline simdjson2_result<std::string_view> unescaped_key(bool allow_replacement = false) noexcept;
  template<typename string_type>
  simdjson2_inline error_code unescaped_key(string_type &receiver, bool allow_replacement = false) noexcept;
  simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::raw_json_string> key() noexcept;
  simdjson2_inline simdjson2_result<std::string_view> key_raw_json_token() noexcept;
  simdjson2_inline simdjson2_result<std::string_view> escaped_key() noexcept;
  simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::value> value() noexcept;
};

} // namespace simdjson2

#endif // SIMDJSON2_GENERIC_ONDEMAND_FIELD_H