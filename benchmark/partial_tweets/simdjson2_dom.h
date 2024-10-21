#pragma once

#if SIMDJSON2_EXCEPTIONS

#include "partial_tweets.h"

namespace partial_tweets {

using namespace simdjson2;

struct simdjson2_dom {
  using StringType=std::string_view;

  dom::parser parser{};

  simdjson2_inline uint64_t nullable_int(dom::element element) {
    if (element.is_null()) { return 0; }
    return element;
  }

  bool run(simdjson2::padded_string &json, std::vector<tweet<std::string_view>> &result) {
    for (dom::element tweet : parser.parse(json)["statuses"]) {
      auto user = tweet["user"];
      result.emplace_back(partial_tweets::tweet<std::string_view>{
        tweet["created_at"],
        tweet["id"],
        tweet["text"],
        nullable_int(tweet["in_reply_to_status_id"]),
        { user["id"], user["screen_name"] },
        tweet["retweet_count"],
        tweet["favorite_count"]
      });
    }

    return true;
  }
};

BENCHMARK_TEMPLATE(partial_tweets, simdjson2_dom)->UseManualTime();

} // namespace partial_tweets

#endif // SIMDJSON2_EXCEPTIONS
