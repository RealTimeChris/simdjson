#pragma once

#if SIMDJSON_COMPETITION_JSONIFIER

#include "distinct_user_id.h"

namespace distinct_user_id {

struct user_data {
  int64_t id;
};

struct status_data {
  user_data user;
  std::shared_ptr<status_data> retweeted_status;
};

struct twitter_message {
  std::vector<status_data> statuses{};
};
} // namespace distinct_user_id

namespace jsonifier {
template <> struct core<distinct_user_id::user_data> {
  using value_type = distinct_user_id::user_data;
  static constexpr auto parseValue = createValue<&value_type::id>();
};

template <> struct core<distinct_user_id::status_data> {
  using value_type = distinct_user_id::status_data;
  static constexpr auto parseValue =
      createValue<&value_type::user, &value_type::retweeted_status>();
};

template <> struct core<distinct_user_id::twitter_message> {
  using value_type = distinct_user_id::twitter_message;
  static constexpr auto parseValue = createValue<&value_type::statuses>();
};
}; // namespace jsonifier

namespace distinct_user_id {
jsonifier::jsonifier_core<> parser{};

struct jsonifier_off_duty {

  bool run(simdjson::padded_string &json, std::vector<uint64_t> &result) {
    // Walk the document, parsing as we go
    twitter_message data;
    parser.parseJson<jsonifier::parse_options{.partialRead = true}>(data, json);
    result.reserve(data.statuses.size());
    for (const auto& tweet : data.statuses) {
      // We believe that all statuses have a matching
      // user, and we are willing to throw when they do not.
      result.emplace_back(tweet.user.id);
      // Not all tweets have a "retweeted_status", but when they do
      // we want to go and find the user within.
      auto retweet = tweet.retweeted_status;
      if (retweet) {
        result.emplace_back(retweet->user.id);
      }
    }

    return true;
  }
};

BENCHMARK_TEMPLATE(distinct_user_id, jsonifier_off_duty)->UseManualTime();

} // namespace distinct_user_id

#endif // SIMDJSON_EXCEPTIONS
