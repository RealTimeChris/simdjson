#pragma once

#if SIMDJSON2_EXCEPTIONS

#include "find_tweet.h"

namespace find_tweet {

using namespace simdjson2;

struct simdjson2_ondemand {
  using StringType=std::string_view;

  ondemand::parser parser{};

  bool run(simdjson2::padded_string &json, uint64_t find_id, std::string_view &result) {
    // Walk the document, parsing as we go
    auto doc = parser.iterate(json);
    for (auto tweet : doc.find_field("statuses")) {
      if (uint64_t(tweet.find_field("id")) == find_id) {
        result = tweet.find_field("text");
        return true;
      }
    }
    return false;
  }
};

BENCHMARK_TEMPLATE(find_tweet, simdjson2_ondemand)->UseManualTime();

} // namespace find_tweet

#endif // SIMDJSON2_EXCEPTIONS
