#pragma once

#if SIMDJSON_EXCEPTIONS

#include "distinct_user_id.h"

namespace distinct_user_id {

using namespace simdjson;

struct simdjson_ondemand {
  ondemand::parser parser{};

  bool run(simdjson::padded_string &json, std::vector<uint64_t> &result) {
    // Walk the document, parsing as we go
    auto doc = parser.iterate(json);

    return true;
  }
};

BENCHMARK_TEMPLATE(distinct_user_id, simdjson_ondemand)->UseManualTime();

} // namespace distinct_user_id

#endif // SIMDJSON_EXCEPTIONS
