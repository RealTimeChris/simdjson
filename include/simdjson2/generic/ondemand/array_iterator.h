#ifndef SIMDJSON2_GENERIC_ONDEMAND_ARRAY_ITERATOR_H

#ifndef SIMDJSON2_CONDITIONAL_INCLUDE
#define SIMDJSON2_GENERIC_ONDEMAND_ARRAY_ITERATOR_H
#include "simdjson2/generic/implementation_simdjson2_result_base.h"
#include "simdjson2/generic/ondemand/base.h"
#include "simdjson2/generic/ondemand/value_iterator.h"
#endif // SIMDJSON2_CONDITIONAL_INCLUDE


namespace simdjson2 {
namespace SIMDJSON2_IMPLEMENTATION {
namespace ondemand {

/**
 * A forward-only JSON array.
 *
 * This is an input_iterator, meaning:
 * - It is forward-only
 * - * must be called exactly once per element.
 * - ++ must be called exactly once in between each * (*, ++, *, ++, * ...)
 */
class array_iterator {
public:
  /** Create a new, invalid array iterator. */
  simdjson2_inline array_iterator() noexcept = default;

  //
  // Iterator interface
  //

  /**
   * Get the current element.
   *
   * Part of the std::iterator interface.
   */
  simdjson2_inline simdjson2_result<value> operator*() noexcept; // MUST ONLY BE CALLED ONCE PER ITERATION.
  /**
   * Check if we are at the end of the JSON.
   *
   * Part of the std::iterator interface.
   *
   * @return true if there are no more elements in the JSON array.
   */
  simdjson2_inline bool operator==(const array_iterator &) const noexcept;
  /**
   * Check if there are more elements in the JSON array.
   *
   * Part of the std::iterator interface.
   *
   * @return true if there are more elements in the JSON array.
   */
  simdjson2_inline bool operator!=(const array_iterator &) const noexcept;
  /**
   * Move to the next element.
   *
   * Part of the std::iterator interface.
   */
  simdjson2_inline array_iterator &operator++() noexcept;

private:
  value_iterator iter{};

  simdjson2_inline array_iterator(const value_iterator &iter) noexcept;

  friend class array;
  friend class value;
  friend struct simdjson2_result<array_iterator>;
};

} // namespace ondemand
} // namespace SIMDJSON2_IMPLEMENTATION
} // namespace simdjson2

namespace simdjson2 {

template<>
struct simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::array_iterator> : public SIMDJSON2_IMPLEMENTATION::implementation_simdjson2_result_base<SIMDJSON2_IMPLEMENTATION::ondemand::array_iterator> {
public:
  simdjson2_inline simdjson2_result(SIMDJSON2_IMPLEMENTATION::ondemand::array_iterator &&value) noexcept; ///< @private
  simdjson2_inline simdjson2_result(error_code error) noexcept; ///< @private
  simdjson2_inline simdjson2_result() noexcept = default;

  //
  // Iterator interface
  //

  simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::value> operator*() noexcept; // MUST ONLY BE CALLED ONCE PER ITERATION.
  simdjson2_inline bool operator==(const simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::array_iterator> &) const noexcept;
  simdjson2_inline bool operator!=(const simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::array_iterator> &) const noexcept;
  simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::array_iterator> &operator++() noexcept;
};

} // namespace simdjson2

#endif // SIMDJSON2_GENERIC_ONDEMAND_ARRAY_ITERATOR_H