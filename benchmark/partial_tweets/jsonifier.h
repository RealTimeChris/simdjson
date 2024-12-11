#pragma once

#ifdef SIMDJSON_COMPETITION_JSONIFIER

#include "partial_tweets.h"
#include <string>
namespace partial_tweets {

struct twitter_user_data {
  uint64_t id{};
  std::string screen_name{};
  JSONIFIER_INLINE operator twitter_user<std::string_view>() {
    return {id, static_cast<std::string_view>(screen_name)};
  }
};

struct status_data {
  uint64_t id{};
  std::string created_at{};
  std::string text{};
  std::optional<uint64_t> in_reply_to_status_id{};
  twitter_user_data user{};
  uint64_t retweet_count{};
  uint64_t favorite_count{};
};

struct twitter_message {
  std::vector<status_data> statuses{};
};
} // namespace partial_tweets

template <>
struct jsonifier::core<partial_tweets::twitter_user_data> {
  using value_type = partial_tweets::twitter_user_data ;
  static constexpr auto parseValue =
      createValue<&value_type::id, &value_type::screen_name>();
};

template <> struct jsonifier::core<partial_tweets::status_data> {
  using value_type = partial_tweets::status_data;
  static constexpr auto parseValue =
      createValue<&value_type::created_at, &value_type::id, &value_type::user,
                  &value_type::text,
                  &value_type::in_reply_to_status_id,
                  &value_type::favorite_count, &value_type::retweet_count>();
};

template <> struct jsonifier::core<partial_tweets::twitter_message> {
  using value_type = partial_tweets::twitter_message;
  static constexpr auto parseValue = createValue<&value_type::statuses>();
};

namespace partial_tweets {

struct Jsonifier {
  using StringType = std::string_view;

  jsonifier::jsonifier_core<> parser{};

  static constexpr auto getValue = [](auto &value) {
    if (value.has_value()) {
      return value.value();
    } else {
      return uint64_t{0};
    }
  };
  partial_tweets::twitter_message dataNew;

  bool run(simdjson::padded_string &json,
           std::vector<tweet<std::string_view>> &result) {
    auto resultNew =
        parser.parseJson<jsonifier::parse_options{.partialRead = true}>(dataNew,
                                                                        json);

    for (auto &value : parser.getErrors()) {
      std::cout << "Jsonifier Error: " << value << std::endl;
    }

    for (auto &tweet : dataNew.statuses) {;
      result.emplace_back(partial_tweets::tweet<std::string_view>{
          static_cast<std::string_view>(tweet.created_at),
          tweet.id,
          static_cast<std::string_view>(tweet.text),
          getValue(tweet.in_reply_to_status_id),
          tweet.user.id,tweet.user.screen_name,
          tweet.retweet_count,
          tweet.favorite_count});
    }

    return resultNew;
  }
};

BENCHMARK_TEMPLATE(partial_tweets, Jsonifier)->UseManualTime();

} // namespace partial_tweets

#endif // SIMDJSON_COMPETITION_JSONIFIER
