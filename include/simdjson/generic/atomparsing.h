#ifndef SIMDJSON_GENERIC_ATOMPARSING_H

#ifndef SIMDJSON_CONDITIONAL_INCLUDE
#define SIMDJSON_GENERIC_ATOMPARSING_H
#include "simdjson/generic/base.h"
#include "simdjson/generic/jsoncharutils.h"
#endif // SIMDJSON_CONDITIONAL_INCLUDE

#include <cstring>

namespace simdjson {
namespace SIMDJSON_IMPLEMENTATION {
namespace {
/// @private
namespace atomparsing {

template <size_t length> struct convert_length_to_int {
  static_assert(length <= 8, "Sorry, but that string is too long!");
  using type = std::conditional_t<
      length == 1, char,
      std::conditional_t<
          length <= 2, uint16_t,
          std::conditional_t<length <= 4, uint32_t,
                             std::conditional_t<length <= 8, uint64_t, void>>>>;
};

template <size_t length>
using convert_length_to_int_t = typename convert_length_to_int<length>::type;

template <size_t size, const char string[size]>
constexpr convert_length_to_int_t<size> get_string_as_int() noexcept {
  convert_length_to_int_t<size> returnValue{};
  for (size_t x = 0; x < size; ++x) {
    returnValue |= static_cast<convert_length_to_int_t<size>>(string[x])
                   << x * 8;
  }
  return returnValue;
}

template <size_t size, const char string[size]>
auto strxncmp(const uint8_t *src) {
  static constexpr auto stringInt{get_string_as_int<size, string>()};
  if constexpr (size == 8) {
    uint64_t sourceVal;
    std::memcpy(&sourceVal, src, size);
    return sourceVal ^ stringInt;
  } else if constexpr (size == 7) {
    uint64_t sourceVal{};
    std::memcpy(&sourceVal, src, size);
    return sourceVal ^ stringInt;
  } else if constexpr (size == 6) {
    uint64_t sourceVal{};
    std::memcpy(&sourceVal, src, size);
    return sourceVal ^ stringInt;
  } else if constexpr (size == 5) {
    uint64_t sourceVal{};
    std::memcpy(&sourceVal, src, size);
    return sourceVal ^ stringInt;
  } else if constexpr (size == 4) {
    uint32_t sourceVal;
    std::memcpy(&sourceVal, src, size);
    return sourceVal ^ stringInt;
  } else if constexpr (size == 3) {
    uint32_t sourceVal{};
    std::memcpy(&sourceVal, src, size);
    return sourceVal ^ stringInt;
  } else if constexpr (size == 2) {
    uint16_t sourceVal;
    std::memcpy(&sourceVal, src, size);
    return sourceVal ^ stringInt;
  } else {
    return src[0] ^ stringInt;
  }
}

simdjson_warn_unused simdjson_inline bool
is_valid_true_atom(const uint8_t *src) {
  static constexpr const char true_val[]{"true"};
  return (strxncmp<4, true_val>(src) |
          jsoncharutils::is_not_structural_or_whitespace(src[4])) == 0;
}

simdjson_warn_unused simdjson_inline bool is_valid_true_atom(const uint8_t *src,
                                                             size_t len) {
  static constexpr const char true_val[]{"true"};
  if (len > 4) {
    return is_valid_true_atom(src);
  } else if (len == 4) {
    return !strxncmp<4, true_val>(src);
  } else {
    return false;
  }
}

simdjson_warn_unused simdjson_inline bool
is_valid_false_atom(const uint8_t *src) {
  static constexpr const char false_val[]{"false"};
  return (strxncmp<5, false_val>(src) |
          jsoncharutils::is_not_structural_or_whitespace(src[5])) == 0;
}

simdjson_warn_unused simdjson_inline bool
is_valid_false_atom(const uint8_t *src, size_t len) {
  static constexpr const char false_val[]{"false"};
  if (len > 5) {
    return is_valid_false_atom(src);
  } else if (len == 5) {
    return !strxncmp<5, false_val>(src);
  } else {
    return false;
  }
}

simdjson_warn_unused simdjson_inline bool
is_valid_null_atom(const uint8_t *src) {
  static constexpr const char null_val[]{"null"};
  return (strxncmp<4, null_val>(src) |
          jsoncharutils::is_not_structural_or_whitespace(src[4])) == 0;
}

simdjson_warn_unused simdjson_inline bool
is_valid_null_atom(const uint8_t *src, size_t len) {
  static constexpr const char null_val[]{"null"};
  if (len > 4) {
    return is_valid_null_atom(src);
  } else if (len == 4) {
    return !strxncmp<4, null_val>(src);
  } else {
    return false;
  }
}

} // namespace atomparsing
} // unnamed namespace
} // namespace SIMDJSON_IMPLEMENTATION
} // namespace simdjson

#endif // SIMDJSON_GENERIC_ATOMPARSING_H
