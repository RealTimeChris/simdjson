#pragma once
#if SIMDJSON_COMPETITION_JSONIFIER
#include "large_random.h"
namespace jsonifier {
template <> struct core<large_random::point> {
  using value_type = large_random::point;
  static constexpr auto parseValue =
      createValue<&value_type::x, &value_type::y, &value_type::z>();
};
}; // namespace jsonifier
namespace large_random {
struct jsonifier_off_duty {
  jsonifier::jsonifier_core<> parser{};
  static constexpr json_benchmark::diff_flags DiffFlags =
      json_benchmark::diff_flags::NONE;
  bool run(simdjson::padded_string &json, std::vector<point> &result) {
    parser.parseJson<jsonifier::parse_options{.partialRead = true,
                                              .minified = false}>(result, json);
    return true;
  }
};
BENCHMARK_TEMPLATE(large_random, jsonifier_off_duty)->UseManualTime();
} // namespace large_random
#endif // SIMDJSON_EXCEPTIONS
