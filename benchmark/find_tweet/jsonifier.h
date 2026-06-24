#pragma once
#if SIMDJSON_COMPETITION_JSONIFIER
#include "find_tweet.h"
namespace find_tweet {
struct tweet_data {
  uint64_t id;
  std::string text;
};
struct twitter_message {
};
}
namespace jsonifier {
template <> struct core<find_tweet::tweet_data> {
  using value_type = find_tweet::tweet_data;
  static constexpr auto parseValue =
      createValue<&value_type::id, &value_type::text>();
};
template <> struct core<find_tweet::twitter_message> {
  using value_type = find_tweet::twitter_message;
  static constexpr auto parseValue = createValue();
};
}; // namespace jsonifier
namespace find_tweet {
struct jsonifier_off_duty {

  jsonifier::jsonifier_core<> parser{};
  using StringType = std::string;
  bool run(simdjson::padded_string &json, uint64_t find_id,
           std::string &result) {
    twitter_message data;
    parser.parseJson<jsonifier::parse_options{.partialRead = true}>(data, json);
    return true;
  }
};
BENCHMARK_TEMPLATE(find_tweet, jsonifier_off_duty)->UseManualTime();
} // namespace find_tweet
#endif // SIMDJSON_EXCEPTIONS
