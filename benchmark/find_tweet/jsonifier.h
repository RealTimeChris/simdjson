#pragma once

#ifdef SIMDJSON_COMPETITION_JSONIFIER

#include "find_tweet.h"

namespace find_tweet {

struct status_data {
  std::string text{};
  int64_t id{};
};

struct twitter_message {
  std::vector<status_data> statuses{};
};
} // namespace find_tweet

template <> struct jsonifier::core<find_tweet::status_data> {
  using value_type = find_tweet::status_data;
  static constexpr auto parseValue =
      createValue<&value_type::id, &value_type::text>();
};

template <> struct jsonifier::core<find_tweet::twitter_message> {
  using value_type = find_tweet::twitter_message;
  static constexpr auto parseValue = createValue<&value_type::statuses>();
};

namespace find_tweet {

struct Jsonifier {
  using StringType = std::string_view;

  jsonifier::jsonifier_core<> parser{};

  bool run(simdjson::padded_string &json, uint64_t find_id,
           std::string_view &result) {
    twitter_message newData;
    parser.parseJson<jsonifier::parse_options{
        .partialRead = true, .knownOrder = false}>(newData, json);
    for (auto &tweet : newData.statuses) {
      if (tweet.id == find_id) {
        result=tweet.text;
        return true;
      }
    }
    return false;
  }
};

BENCHMARK_TEMPLATE(find_tweet, Jsonifier)->UseManualTime();} // namespace find_tweet

#endif // SIMDJSON_COMPETITION_JSONIFIER
