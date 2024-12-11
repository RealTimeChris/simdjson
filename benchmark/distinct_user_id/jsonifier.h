#pragma once

#ifdef SIMDJSON_COMPETITION_JSONIFIER

#include "distinct_user_id.h"

struct user_data {
  int64_t id{};
};

struct status_data {
  double id{};
  user_data user{};
  std::shared_ptr<status_data> retweeted_status{};
};

struct twitter_message {
  std::vector<status_data> statuses{};
};

template <> struct jsonifier::core<user_data> {
  using value_type = user_data;
  static constexpr auto parseValue = createValue<&value_type::id>();
};

template <> struct jsonifier::core<status_data> {
  using value_type = status_data;
  static constexpr auto parseValue =
      createValue<&value_type::id, &value_type::user,
                  &value_type::retweeted_status>();
};

template <> struct jsonifier::core<twitter_message> {
  using value_type = twitter_message;
  static constexpr auto parseValue = createValue<&value_type::statuses>();
};

namespace distinct_user_id {

struct Jsonifier {
  jsonifier::jsonifier_core<> parser{};

  bool run(const std::string_view &json, std::vector<uint64_t> &result) {
    twitter_message newData;

    auto resultNew =
        parser.parseJson<jsonifier::parse_options{.partialRead = true}>(newData,
                                                                        json);

    result.reserve(newData.statuses.size() * 2);

    for (const auto &value : newData.statuses) {
      result.push_back(value.user.id);
      if (value.retweeted_status) {
        result.push_back(value.retweeted_status->user.id);
      }
    }

    return resultNew;
  }
};

BENCHMARK_TEMPLATE(distinct_user_id, Jsonifier)->UseManualTime();

} // namespace distinct_user_id

#endif // SIMDJSON_EXCEPTIONS
