#pragma once

#ifdef SIMDJSON_COMPETITION_JSONIFIER

#include "large_random.h"

template <> struct jsonifier::core<large_random::point> {
  using value_type = large_random::point;
  static constexpr auto parseValue =
      createValue<&value_type::x, &value_type::y, &value_type::z>();
};

namespace large_random {

using namespace jsonifier;

struct Jsonifier {
  static constexpr diff_flags DiffFlags = diff_flags::NONE;

  jsonifier::jsonifier_core<> parser{};

  bool run(const std::string_view &json, std::vector<point> &result) {
    return parser.parseJson<jsonifier::parse_options{.knownOrder = false}>(
        result, json);
  }
};

BENCHMARK_TEMPLATE(large_random, Jsonifier)->UseManualTime();

} // namespace large_random

#endif // SIMDJSON_COMPETITION_JSONIFIER
