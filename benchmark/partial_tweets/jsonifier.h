#pragma once
#if SIMDJSON_COMPETITION_JSONIFIER
#include "partial_tweets.h"
namespace partial_tweets {
struct partial_tweets_doc {
  std::vector<tweet<std::string>> statuses{};
};
} // namespace partial_tweets
namespace jsonifier {
template <> struct core<partial_tweets::twitter_user<std::string>> {
  using value_type = partial_tweets::twitter_user<std::string>;
  static constexpr auto parseValue =
      createValue<&value_type::id, &value_type::screen_name>();
};
template <> struct core<partial_tweets::tweet<std::string>> {
  using value_type = partial_tweets::tweet<std::string>;
  static constexpr auto parseValue = createValue<
      makeJsonEntity<&value_type::created_at, "created_at">(),
      makeJsonEntity<&value_type::id, "id">(),
      makeJsonEntity<&value_type::result, "text">(),
      makeJsonEntity<&value_type::in_reply_to_status_id,
                     "in_reply_to_status_id">(),
      makeJsonEntity<&value_type::user, "user">(),
      makeJsonEntity<&value_type::retweet_count, "retweet_count">(),
      makeJsonEntity<&value_type::favorite_count, "favorite_count">()>();
};
template <> struct core<partial_tweets::partial_tweets_doc> {
  using value_type = partial_tweets::partial_tweets_doc;
  static constexpr auto parseValue = createValue<&value_type::statuses>();
};
}; // namespace jsonifier
namespace partial_tweets {
jsonifier::jsonifier_core<> parser{};
struct jsonifier_ondemand {
  using StringType = std::string;
  bool run(simdjson::padded_string &json,
           std::vector<tweet<std::string>> &result) {
    partial_tweets_doc data;
    parser.parseJson<jsonifier::parse_options{.partialRead = true}>(data, json);
    result = std::move(data.statuses);
    return true;
  }
};
BENCHMARK_TEMPLATE(partial_tweets, jsonifier_ondemand)->UseManualTime();
} // namespace partial_tweets
#endif // SIMDJSON_EXCEPTIONS
