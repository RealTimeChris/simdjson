#ifndef SIMDJSON2_GENERIC_ONDEMAND_OBJECT_ITERATOR_H

#ifndef SIMDJSON2_CONDITIONAL_INCLUDE
#define SIMDJSON2_GENERIC_ONDEMAND_OBJECT_ITERATOR_H
#include "simdjson2/generic/ondemand/base.h"
#include "simdjson2/generic/implementation_simdjson2_result_base.h"
#include "simdjson2/generic/ondemand/value_iterator.h"
#endif // SIMDJSON2_CONDITIONAL_INCLUDE

namespace simdjson2 {
namespace SIMDJSON2_IMPLEMENTATION {
namespace ondemand {

class object_iterator {
public:
  /**
   * Create a new invalid object_iterator.
   *
   * Exists so you can declare a variable and later assign to it before use.
   */
  simdjson2_inline object_iterator() noexcept = default;

  //
  // Iterator interface
  //

  // Reads key and value, yielding them to the user.
  // MUST ONLY BE CALLED ONCE PER ITERATION.
  simdjson2_inline simdjson2_result<field> operator*() noexcept;
  // Assumes it's being compared with the end. true if depth < iter->depth.
  simdjson2_inline bool operator==(const object_iterator &) const noexcept;
  // Assumes it's being compared with the end. true if depth >= iter->depth.
  simdjson2_inline bool operator!=(const object_iterator &) const noexcept;
  // Checks for ']' and ','
  simdjson2_inline object_iterator &operator++() noexcept;

private:
  /**
   * The underlying JSON iterator.
   *
   * PERF NOTE: expected to be elided in favor of the parent document: this is set when the object
   * is first used, and never changes afterwards.
   */
  value_iterator iter{};

  simdjson2_inline object_iterator(const value_iterator &iter) noexcept;
  friend struct simdjson2_result<object_iterator>;
  friend class object;
};

} // namespace ondemand
} // namespace SIMDJSON2_IMPLEMENTATION
} // namespace simdjson2

namespace simdjson2 {

template<>
struct simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::object_iterator> : public SIMDJSON2_IMPLEMENTATION::implementation_simdjson2_result_base<SIMDJSON2_IMPLEMENTATION::ondemand::object_iterator> {
public:
  simdjson2_inline simdjson2_result(SIMDJSON2_IMPLEMENTATION::ondemand::object_iterator &&value) noexcept; ///< @private
  simdjson2_inline simdjson2_result(error_code error) noexcept; ///< @private
  simdjson2_inline simdjson2_result() noexcept = default;

  //
  // Iterator interface
  //

  // Reads key and value, yielding them to the user.
  simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::field> operator*() noexcept; // MUST ONLY BE CALLED ONCE PER ITERATION.
  // Assumes it's being compared with the end. true if depth < iter->depth.
  simdjson2_inline bool operator==(const simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::object_iterator> &) const noexcept;
  // Assumes it's being compared with the end. true if depth >= iter->depth.
  simdjson2_inline bool operator!=(const simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::object_iterator> &) const noexcept;
  // Checks for ']' and ','
  simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::object_iterator> &operator++() noexcept;
};

} // namespace simdjson2

#endif // SIMDJSON2_GENERIC_ONDEMAND_OBJECT_ITERATOR_H