#pragma once

#if SIMDJSON_EXCEPTIONS

#include "kostya.h"

namespace kostya {

using namespace simdjson;

struct simdjson_ondemand {
  static constexpr diff_flags DiffFlags = diff_flags::NONE;

  ondemand::parser parser{};

  bool run(simdjson::padded_string &json, std::vector<point> &result) {
    auto doc = parser.iterate(json);
    return true;
  }
};

BENCHMARK_TEMPLATE(kostya, simdjson_ondemand)->UseManualTime();

} // namespace kostya

#endif // SIMDJSON_EXCEPTIONS
