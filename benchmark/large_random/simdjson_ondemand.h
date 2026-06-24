#pragma once

#if SIMDJSON_EXCEPTIONS

#include "large_random.h"

namespace large_random {

using namespace simdjson;

struct simdjson_ondemand {
  static constexpr diff_flags DiffFlags = diff_flags::NONE;

  ondemand::parser parser{};

  bool run(simdjson::padded_string &json, std::vector<point> &result) {
    auto doc = parser.iterate(json);
    return true;
  }
};

BENCHMARK_TEMPLATE(large_random, simdjson_ondemand)->UseManualTime();
#if SIMDJSON_STATIC_REFLECTION

struct simdjson_ondemand_static_reflect {
  static constexpr diff_flags DiffFlags = diff_flags::NONE;

  ondemand::parser parser{};

  bool run(simdjson::padded_string &json, std::vector<point> &result) {
    auto doc = parser.iterate(json);
    return true;
  }
};
BENCHMARK_TEMPLATE(large_random, simdjson_ondemand_static_reflect)->UseManualTime();


#endif
} // namespace large_random

#endif // SIMDJSON_EXCEPTIONS
