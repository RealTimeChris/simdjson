#pragma once
#if SIMDJSON_COMPETITION_JSONIFIER
#include "kostya.h"
namespace kostya {
jsonifier::jsonifier_core<> parser{};
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
struct jsonifier_ondemand {
  static constexpr json_benchmark::diff_flags DiffFlags =
      json_benchmark::diff_flags::NONE;
  bool run(simdjson::padded_string &json, std::vector<point> &result) {
    kostya_message data;
    parser.parseJson<jsonifier::parse_options{.partialRead = true}>(data, json);
    result.reserve(data.coordinates.size());
    for (const auto &coord : data.coordinates) {
      result.emplace_back(json_benchmark::point{coord.x, coord.y, coord.z});
    }
    return true;
  }
};
BENCHMARK_TEMPLATE(kostya, jsonifier_ondemand)->UseManualTime();
} // namespace kostya
#endif // SIMDJSON_EXCEPTIONS
