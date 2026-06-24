#pragma once
#if SIMDJSON_COMPETITION_JSONIFIER
#include "kostya.h"
namespace kostya {
struct coordinate_data {
  double x;
  double y;
  double z;
};
struct kostya_message {
  std::vector<coordinate_data> coordinates{};
};
} // namespace kostya
namespace jsonifier {
template <> struct core<kostya::coordinate_data> {
  using value_type = kostya::coordinate_data;
  static constexpr auto parseValue =
      createValue<&value_type::x, &value_type::y, &value_type::z>();
};
template <> struct core<kostya::kostya_message> {
  using value_type = kostya::kostya_message;
  static constexpr auto parseValue = createValue<&value_type::coordinates>();
};
}; // namespace jsonifier
namespace kostya {
struct jsonifier_off_duty {
  jsonifier::jsonifier_core<> parser{};
  static constexpr json_benchmark::diff_flags DiffFlags =
      json_benchmark::diff_flags::NONE;
  bool run(simdjson::padded_string &json, std::vector<point> &result) {
    kostya_message data;
    parser.parseJson<jsonifier::parse_options{.partialRead = true}>(data, json);
    return true;
  }
};
BENCHMARK_TEMPLATE(kostya, jsonifier_off_duty)->UseManualTime();
} // namespace kostya
#endif // SIMDJSON_EXCEPTIONS
