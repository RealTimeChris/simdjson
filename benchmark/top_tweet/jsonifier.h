#pragma once
#if SIMDJSON_COMPETITION_JSONIFIER
#include "top_tweet.h"
namespace top_tweet {
struct top_tweet_user {
  std::string screen_name{};
};
struct top_tweet_tweet {
  std::string text{};
  top_tweet_user user{};
  int64_t retweet_count{};
};
struct top_tweet_doc {
};
} // namespace top_tweet
namespace jsonifier {
template <> struct core<top_tweet::top_tweet_user> {
  using value_type = top_tweet::top_tweet_user;
  static constexpr auto parseValue =
      createValue<makeJsonEntity<&value_type::screen_name, "screen_name">()>();
};
template <> struct core<top_tweet::top_tweet_tweet> {
  using value_type = top_tweet::top_tweet_tweet;
  static constexpr auto parseValue = createValue<
      makeJsonEntity<&value_type::text, "text">(),
      makeJsonEntity<&value_type::user, "user">(),
      makeJsonEntity<&value_type::retweet_count, "retweet_count">()>();
};
template <> struct core<top_tweet::top_tweet_doc> {
  using value_type = top_tweet::top_tweet_doc;
  static constexpr auto parseValue =
      createValue();
};
}; // namespace jsonifier
namespace top_tweet {
struct jsonifier_off_duty {
  jsonifier::jsonifier_core<> parser{};
  using StringType = std::string;
  bool run(simdjson::padded_string &json, int64_t max_retweet_count,
           top_tweet_result<std::string> &result) {
    result.retweet_count = -1;
    top_tweet_doc data;
    parser.parseJson<jsonifier::parse_options{.partialRead = true}>(data, json);
    return true;
  }
};
BENCHMARK_TEMPLATE(top_tweet, jsonifier_off_duty)->UseManualTime();
} // namespace top_tweet
#endif // SIMDJSON_EXCEPTIONS
