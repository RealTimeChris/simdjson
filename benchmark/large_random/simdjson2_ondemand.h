#pragma once

#if SIMDJSON2_EXCEPTIONS

#include "large_random.h"

namespace large_random {

using namespace simdjson2;

struct simdjson2_ondemand {
  static constexpr diff_flags DiffFlags = diff_flags::NONE;

  ondemand::parser parser{};

  bool run(simdjson2::padded_string &json, std::vector<point> &result) {
    auto doc = parser.iterate(json);
    for (ondemand::object coord : doc) {
      result.emplace_back(json_benchmark::point{coord.find_field("x"), coord.find_field("y"), coord.find_field("z")});
    }
    return true;
  }
};

BENCHMARK_TEMPLATE(large_random, simdjson2_ondemand)->UseManualTime();

} // namespace large_random

#endif // SIMDJSON2_EXCEPTIONS
