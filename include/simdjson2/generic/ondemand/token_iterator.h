#ifndef SIMDJSON2_GENERIC_ONDEMAND_TOKEN_ITERATOR_H

#ifndef SIMDJSON2_CONDITIONAL_INCLUDE
#define SIMDJSON2_GENERIC_ONDEMAND_TOKEN_ITERATOR_H
#include "simdjson2/generic/ondemand/base.h"
#include "simdjson2/generic/implementation_simdjson2_result_base.h"
#include "simdjson2/generic/ondemand/logger.h"
#endif // SIMDJSON2_CONDITIONAL_INCLUDE

namespace simdjson2 {
namespace SIMDJSON2_IMPLEMENTATION {
namespace ondemand {

/**
 * Iterates through JSON tokens (`{` `}` `[` `]` `,` `:` `"<string>"` `123` `true` `false` `null`)
 * detected by stage 1.
 *
 * @private This is not intended for external use.
 */
class token_iterator {
public:
  /**
   * Create a new invalid token_iterator.
   *
   * Exists so you can declare a variable and later assign to it before use.
   */
  simdjson2_inline token_iterator() noexcept = default;
  simdjson2_inline token_iterator(token_iterator &&other) noexcept = default;
  simdjson2_inline token_iterator &operator=(token_iterator &&other) noexcept = default;
  simdjson2_inline token_iterator(const token_iterator &other) noexcept = default;
  simdjson2_inline token_iterator &operator=(const token_iterator &other) noexcept = default;

  /**
   * Advance to the next token (returning the current one).
   */
  simdjson2_inline const uint8_t *return_current_and_advance() noexcept;
  /**
   * Reports the current offset in bytes from the start of the underlying buffer.
   */
  simdjson2_inline uint32_t current_offset() const noexcept;
  /**
   * Get the JSON text for a given token (relative).
   *
   * This is not null-terminated; it is a view into the JSON.
   *
   * @param delta The relative position of the token to retrieve. e.g. 0 = current token,
   *              1 = next token, -1 = prev token.
   *
   * TODO consider a string_view, assuming the length will get stripped out by the optimizer when
   * it is not used...
   */
  simdjson2_inline const uint8_t *peek(int32_t delta=0) const noexcept;
  /**
   * Get the maximum length of the JSON text for a given token.
   *
   * The length will include any whitespace at the end of the token.
   *
   * @param delta The relative position of the token to retrieve. e.g. 0 = current token,
   *              1 = next token, -1 = prev token.
   */
  simdjson2_inline uint32_t peek_length(int32_t delta=0) const noexcept;

  /**
   * Get the JSON text for a given token.
   *
   * This is not null-terminated; it is a view into the JSON.
   *
   * @param position The position of the token.
   *
   */
  simdjson2_inline const uint8_t *peek(token_position position) const noexcept;
  /**
   * Get the maximum length of the JSON text for a given token.
   *
   * The length will include any whitespace at the end of the token.
   *
   * @param position The position of the token.
   */
  simdjson2_inline uint32_t peek_length(token_position position) const noexcept;
  /**
   * Get the maximum length of the JSON text for a root token.
   *
   * The length will include any whitespace at the end of the token.
   *
   * @param position The position of the token (start of the document).
   */
  simdjson2_inline uint32_t peek_root_length(token_position position) const noexcept;
  /**
   * Return the current index.
   */
  simdjson2_inline token_position position() const noexcept;
  /**
   * Reset to a previously saved index.
   */
  simdjson2_inline void set_position(token_position target_position) noexcept;

  // NOTE: we don't support a full C++ iterator interface, because we expect people to make
  // different calls to advance the iterator based on *their own* state.

  simdjson2_inline bool operator==(const token_iterator &other) const noexcept;
  simdjson2_inline bool operator!=(const token_iterator &other) const noexcept;
  simdjson2_inline bool operator>(const token_iterator &other) const noexcept;
  simdjson2_inline bool operator>=(const token_iterator &other) const noexcept;
  simdjson2_inline bool operator<(const token_iterator &other) const noexcept;
  simdjson2_inline bool operator<=(const token_iterator &other) const noexcept;

protected:
  simdjson2_inline token_iterator(const uint8_t *buf, token_position position) noexcept;

  /**
   * Get the index of the JSON text for a given token (relative).
   *
   * This is not null-terminated; it is a view into the JSON.
   *
   * @param delta The relative position of the token to retrieve. e.g. 0 = current token,
   *              1 = next token, -1 = prev token.
   */
  simdjson2_inline uint32_t peek_index(int32_t delta=0) const noexcept;
  /**
   * Get the index of the JSON text for a given token.
   *
   * This is not null-terminated; it is a view into the JSON.
   *
   * @param position The position of the token.
   *
   */
  simdjson2_inline uint32_t peek_index(token_position position) const noexcept;

  const uint8_t *buf{};
  token_position _position{};

  friend class json_iterator;
  friend class value_iterator;
  friend class object;
  template <typename... Args>
  friend simdjson2_inline void logger::log_line(const json_iterator &iter, const char *title_prefix, const char *title, std::string_view detail, int delta, int depth_delta, logger::log_level level, Args&&... args) noexcept;
  template <typename... Args>
  friend simdjson2_inline void logger::log_line(const json_iterator &iter, token_position index, depth_t depth, const char *title_prefix, const char *title, std::string_view detail, logger::log_level level, Args&&... args) noexcept;
};

} // namespace ondemand
} // namespace SIMDJSON2_IMPLEMENTATION
} // namespace simdjson2

namespace simdjson2 {

template<>
struct simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::token_iterator> : public SIMDJSON2_IMPLEMENTATION::implementation_simdjson2_result_base<SIMDJSON2_IMPLEMENTATION::ondemand::token_iterator> {
public:
  simdjson2_inline simdjson2_result(SIMDJSON2_IMPLEMENTATION::ondemand::token_iterator &&value) noexcept; ///< @private
  simdjson2_inline simdjson2_result(error_code error) noexcept; ///< @private
  simdjson2_inline simdjson2_result() noexcept = default;
  simdjson2_inline ~simdjson2_result() noexcept = default; ///< @private
};

} // namespace simdjson2

#endif // SIMDJSON2_GENERIC_ONDEMAND_TOKEN_ITERATOR_H