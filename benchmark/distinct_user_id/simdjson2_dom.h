#pragma once

#if SIMDJSON2_EXCEPTIONS

#include "distinct_user_id.h"

namespace distinct_user_id {

using namespace simdjson2;

struct simdjson2_dom {
  dom::parser parser{};

  bool run(simdjson2::padded_string &json, std::vector<uint64_t> &result) {
    // Walk the document, parsing as we go
    auto doc = parser.parse(json);
    for (dom::object tweet : doc["statuses"]) {
      // We believe that all statuses have a matching
      // user, and we are willing to throw when they do not.
      result.push_back(tweet["user"]["id"]);
      // Not all tweets have a "retweeted_status", but when they do
      // we want to go and find the user within.
      auto retweet = tweet["retweeted_status"];
      if (retweet.error() != NO_SUCH_FIELD) {
        result.push_back(retweet["user"]["id"]);
      }
    }
    return true;
  }
};

BENCHMARK_TEMPLATE(distinct_user_id, simdjson2_dom)->UseManualTime();

} // namespace distinct_user_id

#endif // SIMDJSON2_EXCEPTIONS