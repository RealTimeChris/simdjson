#pragma once

#ifdef SIMDJSON_COMPETITION_JSONIFIER

#include "top_tweet.h"

namespace top_tweet {

  struct user_data {
    std::string screen_name{};
  };

  struct status_data {
    std::string text{};
    user_data user{};
    int64_t retweet_count{};
  };

  struct twitter_message {
    std::vector<status_data> statuses{};
  };
}

template <> struct jsonifier::core<top_tweet::user_data> {
  using value_type = top_tweet::user_data;
  static constexpr auto parseValue = createValue<&value_type::screen_name>();
};

template <> struct jsonifier::core<top_tweet::status_data> {
  using value_type = top_tweet::status_data;
  static constexpr auto parseValue =
      createValue<&value_type::text, &value_type::user,
                  &value_type::retweet_count>();
};

template <> struct jsonifier::core<top_tweet::twitter_message> {
  using value_type = top_tweet::twitter_message;
  static constexpr auto parseValue = createValue<&value_type::statuses>();
};

namespace top_tweet {

struct Jsonifier {
  using StringType = std::string;

  jsonifier::jsonifier_core<> parser{};

  bool run(const std::string_view &json, int64_t max_retweet_count,
           top_tweet_result<std::string> &result) {
    twitter_message newData;
    result.retweet_count = -1;
    parser.parseJson<jsonifier::parse_options{
        .partialRead = true, .knownOrder = false}>(newData, json);
    for (auto &tweet : newData.statuses) {
      if (tweet.retweet_count <= max_retweet_count &&
          tweet.retweet_count >= result.retweet_count) {
        result.retweet_count = tweet.retweet_count;
        result.text = std::move(tweet.text);
        result.screen_name = std::move(tweet.user.screen_name);
      }
    }
    return result.retweet_count != -1;
  }
};

BENCHMARK_TEMPLATE(top_tweet, Jsonifier)->UseManualTime();

struct Jsonifier_forward_only {
  using StringType = std::string;

  jsonifier::jsonifier_core<> parser{};

  bool run(const std::string_view &json, int64_t max_retweet_count,
           top_tweet_result<std::string> &result) {
    twitter_message newData;
    result.retweet_count = -1;
    parser.parseJson<jsonifier::parse_options{
        .partialRead = true, .knownOrder = false}>(newData, json);
    for (auto &tweet : newData.statuses) {
      if (tweet.retweet_count <= max_retweet_count &&
          tweet.retweet_count >= result.retweet_count) {
        result.retweet_count = tweet.retweet_count;
        result.text = tweet.text;
        result.screen_name = tweet.user.screen_name;
      }
    }
    return result.retweet_count != -1;
  }
};

BENCHMARK_TEMPLATE(top_tweet, Jsonifier_forward_only)->UseManualTime();
} // namespace top_tweet

#endif // SIMDJSON_COMPETITION_JSONIFIER
