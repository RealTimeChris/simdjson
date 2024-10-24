#pragma once

#if SIMDJSON2_EXCEPTIONS

#include "kostya.h"

namespace kostya {

using namespace simdjson2;

struct simdjson2_ondemand {
  static constexpr diff_flags DiffFlags = diff_flags::NONE;

  ondemand::parser parser{};

  bool run(simdjson2::padded_string &json, std::vector<point> &result) {
    auto doc = parser.iterate(json);
    for (ondemand::object point : doc.find_field("coordinates")) {
      result.emplace_back(json_benchmark::point{point.find_field("x"), point.find_field("y"), point.find_field("z")});
    }
    return true;
  }
};

BENCHMARK_TEMPLATE(kostya, simdjson2_ondemand)->UseManualTime();

} // namespace kostya

#endif // SIMDJSON2_EXCEPTIONS
