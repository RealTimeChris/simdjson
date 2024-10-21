#ifndef SIMDJSON2_GENERIC_ONDEMAND_OBJECT_INL_H

#ifndef SIMDJSON2_CONDITIONAL_INCLUDE
#define SIMDJSON2_GENERIC_ONDEMAND_OBJECT_INL_H
#include "simdjson2/generic/ondemand/base.h"
#include "simdjson2/generic/ondemand/field.h"
#include "simdjson2/generic/ondemand/object.h"
#include "simdjson2/generic/ondemand/object_iterator.h"
#include "simdjson2/generic/ondemand/raw_json_string.h"
#include "simdjson2/generic/ondemand/json_iterator.h"
#include "simdjson2/generic/ondemand/value-inl.h"
#endif // SIMDJSON2_CONDITIONAL_INCLUDE

namespace simdjson2 {
namespace SIMDJSON2_IMPLEMENTATION {
namespace ondemand {

simdjson2_inline simdjson2_result<value> object::find_field_unordered(const std::string_view key) & noexcept {
  bool has_value;
  SIMDJSON2_TRY( iter.find_field_unordered_raw(key).get(has_value) );
  if (!has_value) {
    logger::log_line(iter.json_iter(), "ERROR: ", "Cannot find key %.*s", "", -1, 0, logger::log_level::error, static_cast<int>(key.size()), key.data());
    return NO_SUCH_FIELD;
  }
  return value(iter.child());
}
simdjson2_inline simdjson2_result<value> object::find_field_unordered(const std::string_view key) && noexcept {
  bool has_value;
  SIMDJSON2_TRY( iter.find_field_unordered_raw(key).get(has_value) );
  if (!has_value) {
    logger::log_line(iter.json_iter(), "ERROR: ", "Cannot find key %.*s", "", -1, 0, logger::log_level::error, static_cast<int>(key.size()), key.data());
    return NO_SUCH_FIELD;
  }
  return value(iter.child());
}
simdjson2_inline simdjson2_result<value> object::operator[](const std::string_view key) & noexcept {
  return find_field_unordered(key);
}
simdjson2_inline simdjson2_result<value> object::operator[](const std::string_view key) && noexcept {
  return std::forward<object>(*this).find_field_unordered(key);
}
simdjson2_inline simdjson2_result<value> object::find_field(const std::string_view key) & noexcept {
  bool has_value;
  SIMDJSON2_TRY( iter.find_field_raw(key).get(has_value) );
  if (!has_value) {
    logger::log_line(iter.json_iter(), "ERROR: ", "Cannot find key %.*s", "", -1, 0, logger::log_level::error, static_cast<int>(key.size()), key.data());
    return NO_SUCH_FIELD;
  }
  return value(iter.child());
}
simdjson2_inline simdjson2_result<value> object::find_field(const std::string_view key) && noexcept {
  bool has_value;
  SIMDJSON2_TRY( iter.find_field_raw(key).get(has_value) );
  if (!has_value) {
    logger::log_line(iter.json_iter(), "ERROR: ", "Cannot find key %.*s", "", -1, 0, logger::log_level::error, static_cast<int>(key.size()), key.data());
    return NO_SUCH_FIELD;
  }
  return value(iter.child());
}

simdjson2_inline simdjson2_result<object> object::start(value_iterator &iter) noexcept {
  SIMDJSON2_TRY( iter.start_object().error() );
  return object(iter);
}
simdjson2_inline simdjson2_result<object> object::start_root(value_iterator &iter) noexcept {
  SIMDJSON2_TRY( iter.start_root_object().error() );
  return object(iter);
}
simdjson2_inline error_code object::consume() noexcept {
  if(iter.is_at_key()) {
    /**
     * whenever you are pointing at a key, calling skip_child() is
     * unsafe because you will hit a string and you will assume that
     * it is string value, and this mistake will lead you to make bad
     * depth computation.
     */
    /**
     * We want to 'consume' the key. We could really
     * just do _json_iter->return_current_and_advance(); at this
     * point, but, for clarity, we will use the high-level API to
     * eat the key. We assume that the compiler optimizes away
     * most of the work.
     */
    simdjson2_unused raw_json_string actual_key;
    auto error = iter.field_key().get(actual_key);
    if (error) { iter.abandon(); return error; };
    // Let us move to the value while we are at it.
    if ((error = iter.field_value())) { iter.abandon(); return error; }
  }
  auto error_skip = iter.json_iter().skip_child(iter.depth()-1);
  if(error_skip) { iter.abandon(); }
  return error_skip;
}

simdjson2_inline simdjson2_result<std::string_view> object::raw_json() noexcept {
  const uint8_t * starting_point{iter.peek_start()};
  auto error = consume();
  if(error) { return error; }
  const uint8_t * final_point{iter._json_iter->peek()};
  return std::string_view(reinterpret_cast<const char*>(starting_point), size_t(final_point - starting_point));
}

simdjson2_inline simdjson2_result<object> object::started(value_iterator &iter) noexcept {
  SIMDJSON2_TRY( iter.started_object().error() );
  return object(iter);
}

simdjson2_inline object object::resume(const value_iterator &iter) noexcept {
  return iter;
}

simdjson2_inline object::object(const value_iterator &_iter) noexcept
  : iter{_iter}
{
}

simdjson2_inline simdjson2_result<object_iterator> object::begin() noexcept {
#if SIMDJSON2_DEVELOPMENT_CHECKS
  if (!iter.is_at_iterator_start()) { return OUT_OF_ORDER_ITERATION; }
#endif
  return object_iterator(iter);
}
simdjson2_inline simdjson2_result<object_iterator> object::end() noexcept {
  return object_iterator(iter);
}

inline simdjson2_result<value> object::at_pointer(std::string_view json_pointer) noexcept {
  if (json_pointer[0] != '/') { return INVALID_JSON_POINTER; }
  json_pointer = json_pointer.substr(1);
  size_t slash = json_pointer.find('/');
  std::string_view key = json_pointer.substr(0, slash);
  // Grab the child with the given key
  simdjson2_result<value> child;

  // If there is an escape character in the key, unescape it and then get the child.
  size_t escape = key.find('~');
  if (escape != std::string_view::npos) {
    // Unescape the key
    std::string unescaped(key);
    do {
      switch (unescaped[escape+1]) {
        case '0':
          unescaped.replace(escape, 2, "~");
          break;
        case '1':
          unescaped.replace(escape, 2, "/");
          break;
        default:
          return INVALID_JSON_POINTER; // "Unexpected ~ escape character in JSON pointer");
      }
      escape = unescaped.find('~', escape+1);
    } while (escape != std::string::npos);
    child = find_field(unescaped);  // Take note find_field does not unescape keys when matching
  } else {
    child = find_field(key);
  }
  if(child.error()) {
    return child; // we do not continue if there was an error
  }
  // If there is a /, we have to recurse and look up more of the path
  if (slash != std::string_view::npos) {
    child = child.at_pointer(json_pointer.substr(slash));
  }
  return child;
}

inline simdjson2_result<value> object::at_path(std::string_view json_path) noexcept {
  auto json_pointer = json_path_to_pointer_conversion(json_path);
  if (json_pointer == "-1") {
    return INVALID_JSON_POINTER;
  }
  return at_pointer(json_pointer);
}

simdjson2_inline simdjson2_result<size_t> object::count_fields() & noexcept {
  size_t count{0};
  // Important: we do not consume any of the values.
  for(simdjson2_unused auto v : *this) { count++; }
  // The above loop will always succeed, but we want to report errors.
  if(iter.error()) { return iter.error(); }
  // We need to move back at the start because we expect users to iterate through
  // the object after counting the number of elements.
  iter.reset_object();
  return count;
}

simdjson2_inline simdjson2_result<bool> object::is_empty() & noexcept {
  bool is_not_empty;
  auto error = iter.reset_object().get(is_not_empty);
  if(error) { return error; }
  return !is_not_empty;
}

simdjson2_inline simdjson2_result<bool> object::reset() & noexcept {
  return iter.reset_object();
}

} // namespace ondemand
} // namespace SIMDJSON2_IMPLEMENTATION
} // namespace simdjson2

namespace simdjson2 {

simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::object>::simdjson2_result(SIMDJSON2_IMPLEMENTATION::ondemand::object &&value) noexcept
    : implementation_simdjson2_result_base<SIMDJSON2_IMPLEMENTATION::ondemand::object>(std::forward<SIMDJSON2_IMPLEMENTATION::ondemand::object>(value)) {}
simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::object>::simdjson2_result(error_code error) noexcept
    : implementation_simdjson2_result_base<SIMDJSON2_IMPLEMENTATION::ondemand::object>(error) {}

simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::object_iterator> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::object>::begin() noexcept {
  if (error()) { return error(); }
  return first.begin();
}
simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::object_iterator> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::object>::end() noexcept {
  if (error()) { return error(); }
  return first.end();
}
simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::value> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::object>::find_field_unordered(std::string_view key) & noexcept {
  if (error()) { return error(); }
  return first.find_field_unordered(key);
}
simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::value> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::object>::find_field_unordered(std::string_view key) && noexcept {
  if (error()) { return error(); }
  return std::forward<SIMDJSON2_IMPLEMENTATION::ondemand::object>(first).find_field_unordered(key);
}
simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::value> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::object>::operator[](std::string_view key) & noexcept {
  if (error()) { return error(); }
  return first[key];
}
simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::value> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::object>::operator[](std::string_view key) && noexcept {
  if (error()) { return error(); }
  return std::forward<SIMDJSON2_IMPLEMENTATION::ondemand::object>(first)[key];
}
simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::value> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::object>::find_field(std::string_view key) & noexcept {
  if (error()) { return error(); }
  return first.find_field(key);
}
simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::value> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::object>::find_field(std::string_view key) && noexcept {
  if (error()) { return error(); }
  return std::forward<SIMDJSON2_IMPLEMENTATION::ondemand::object>(first).find_field(key);
}

simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::value> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::object>::at_pointer(std::string_view json_pointer) noexcept {
  if (error()) { return error(); }
  return first.at_pointer(json_pointer);
}

simdjson2_inline simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::value> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::object>::at_path(
    std::string_view json_path) noexcept {
  if (error()) {
    return error();
  }
  return first.at_path(json_path);
}

inline simdjson2_result<bool> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::object>::reset() noexcept {
  if (error()) { return error(); }
  return first.reset();
}

inline simdjson2_result<bool> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::object>::is_empty() noexcept {
  if (error()) { return error(); }
  return first.is_empty();
}

simdjson2_inline  simdjson2_result<size_t> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::object>::count_fields() & noexcept {
  if (error()) { return error(); }
  return first.count_fields();
}

simdjson2_inline  simdjson2_result<std::string_view> simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::object>::raw_json() noexcept {
  if (error()) { return error(); }
  return first.raw_json();
}
} // namespace simdjson2

#endif // SIMDJSON2_GENERIC_ONDEMAND_OBJECT_INL_H
