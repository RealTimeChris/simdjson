#ifndef SIMDJSON2_GENERIC_ONDEMAND_ARRAY_ITERATOR_INL_H

#ifndef SIMDJSON2_CONDITIONAL_INCLUDE
#define SIMDJSON2_GENERIC_ONDEMAND_ARRAY_ITERATOR_INL_H
#include "simdjson2/generic/ondemand/base.h"
#include "simdjson2/generic/ondemand/array_iterator.h"
#include "simdjson2/generic/ondemand/value-inl.h"
#include "simdjson2/generic/ondemand/value_iterator-inl.h"
#endif // SIMDJSON2_CONDITIONAL_INCLUDE

namespace simdjson2 {
namespace SIMDJSON2_IMPLEMENTATION {
namespace ondemand {

simdjson2_inline array_iterator::array_iterator(const value_iterator &_iter) noexcept
  : iter{_iter}
{}

simdjson2_inline simdjson2_result<value> array_iterator::operator*() noexcept {
  if (iter.error()) { iter.abandon(); return iter.error(); }
  return value(iter.child());
}
simdjson2_inline bool array_iterator::operator==(const array_iterator &other) const noexcept {
  return !(*this != other);
}
simdjson2_inline bool array_iterator::operator!=(const array_iterator &) const noexcept {
  return iter.is_open();
}
simdjson2_inline array_iterator &array_iterator::operator++() noexcept {
  error_code error;
  // PERF NOTE this is a safety rail ... users should exit loops as soon as they receive an error, so we'll never get here.
  // However, it does not seem to make a perf difference, so we add it out of an abundance of caution.
  if (( error = iter.error() )) { return *this; }
  if (( error = iter.skip_child() )) { return *this; }
  if (( error = iter.has_next_element().error() )) { return *this; }
  return *this;
}

} // namespace ondemand
} // namespace SIMDJSON2_IMPLEMENTATION
} // namespace simdjson2

namespace simdjson2 {

simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::array_iterator>::simdjson2_result(
  SIMDJSON2_IMPLEMENTATION::ondemand::array_iterator &&value
) noexcept
  : SIMDJSON2_IMPLEMENTATION::implementation_simdjson2_result_base<SIMDJSON2_IMPLEMENTATION::ondemand::array_iterator>(std::forward<SIMDJSON2_IMPLEMENTATION::ondemand::array_iterator>(value))
{
  first.iter.assert_is_valid();
}
simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::array_iterator>::simdjson2_result(error_code error) noexcept
  : SIMDJSON2_IMPLEMENTATION::implementation_simdjson2_result_base<SIMDJSON2_IMPLEMENTATION::ondemand::array_iterator>({}, error)
{
}

simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::value> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::array_iterator>::operator*() noexcept {
  if (error()) { return error(); }
  return *first;
}
simdjson2_inline bool simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::array_iterator>::operator==(const simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::array_iterator> &other) const noexcept {
  if (!first.iter.is_valid()) { return !error(); }
  return first == other.first;
}
simdjson2_inline bool simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::array_iterator>::operator!=(const simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::array_iterator> &other) const noexcept {
  if (!first.iter.is_valid()) { return error(); }
  return first != other.first;
}
simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::array_iterator> &simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::array_iterator>::operator++() noexcept {
  // Clear the error if there is one, so we don't yield it twice
  if (error()) { second = SUCCESS; return *this; }
  ++(first);
  return *this;
}

} // namespace simdjson2

#endif // SIMDJSON2_GENERIC_ONDEMAND_ARRAY_ITERATOR_INL_H