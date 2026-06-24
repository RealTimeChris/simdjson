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
  std::vector<top_tweet_tweet> statuses{};
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
      createValue<makeJsonEntity<&value_type::statuses, "statuses">()>();
};
}; // namespace jsonifier
namespace top_tweet {
jsonifier::jsonifier_core<> parser{};
struct jsonifier_ondemand {
  using StringType = std::string;
  bool run(simdjson::padded_string &json, int64_t max_retweet_count,
           top_tweet_result<std::string> &result) {
    result.retweet_count = -1;
    top_tweet_doc data;
    parser.parseJson<jsonifier::parse_options{.partialRead = true}>(data, json);
    top_tweet_tweet *winner = nullptr;
    for (auto &tweet : data.statuses) {
      if (tweet.retweet_count <= max_retweet_count &&
          tweet.retweet_count >= result.retweet_count) {
        result.retweet_count = tweet.retweet_count;
        winner = &tweet;
      }
    }
    if (winner) {
      result.text = std::move(winner->text);
      result.screen_name = std::move(winner->user.screen_name);
    }
    return result.retweet_count != -1;
  }
};
BENCHMARK_TEMPLATE(top_tweet, jsonifier_ondemand)->UseManualTime();
} // namespace top_tweet
#endif // SIMDJSON_EXCEPTIONS
