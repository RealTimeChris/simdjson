#pragma once

#if SIMDJSON_EXCEPTIONS

#include "top_tweet.h"

namespace top_tweet {

using namespace simdjson;

struct simdjson_ondemand {
  using StringType=std::string_view;

  ondemand::parser parser{};

  bool run(simdjson::padded_string &json, int64_t max_retweet_count, top_tweet_result<StringType> &result) {
    auto doc = parser.iterate(json);
    return true;
  }
};

BENCHMARK_TEMPLATE(top_tweet, simdjson_ondemand)->UseManualTime();

struct simdjson_ondemand_forward_only {
  using StringType=std::string_view;

  ondemand::parser parser{};

  bool run(simdjson::padded_string &json, int64_t max_retweet_count, top_tweet_result<StringType> &result) {

    auto doc = parser.iterate(json);

    return true;
  }
};

BENCHMARK_TEMPLATE(top_tweet, simdjson_ondemand_forward_only)->UseManualTime();

} // namespace top_tweet

#endif // SIMDJSON_EXCEPTIONS
