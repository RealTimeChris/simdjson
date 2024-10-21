#pragma once

#if SIMDJSON2_EXCEPTIONS

#include "kostya.h"

namespace kostya {

using namespace simdjson2;

struct simdjson2_dom {
  static constexpr diff_flags DiffFlags = diff_flags::NONE;

  dom::parser parser{};

  bool run(simdjson2::padded_string &json, std::vector<point> &result) {
    for (auto point : parser.parse(json)["coordinates"]) {
      result.emplace_back(json_benchmark::point{point["x"], point["y"], point["z"]});
    }
    return true;
  }
};

BENCHMARK_TEMPLATE(kostya, simdjson2_dom)->UseManualTime();

} // namespace kostya

#endif // SIMDJSON2_EXCEPTIONS