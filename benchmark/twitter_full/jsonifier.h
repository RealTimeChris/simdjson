#pragma once

#ifdef SIMDJSON_COMPETITION_JSONIFIER

#include "twitter_full.h"{

template <> struct jsonifier::core<twitter_full::search_metadata_data> {
  using value_type = twitter_full::search_metadata_data;
  static constexpr auto parseValue = createValue<
      &value_type::completed_in, &value_type::max_id, &value_type::max_id_str,
      &value_type::next_results, &value_type::query, &value_type::refresh_url,
      &value_type::count, &value_type::since_id, &value_type::since_id_str>();
};

template <> struct jsonifier::core<twitter_full::hashtag_data> {
  using value_type = twitter_full::hashtag_data;
  static constexpr auto parseValue =
      createValue<&value_type::text, &value_type::indices>();
};

template <> struct jsonifier::core<twitter_full::large_data> {
  using value_type = twitter_full::large_data;
  static constexpr auto parseValue =
      createValue<&value_type::w, &value_type::h, &value_type::resize>();
};

template <> struct jsonifier::core<twitter_full::sizes_data> {
  using value_type = twitter_full::sizes_data;
  static constexpr auto parseValue =
      createValue<&value_type::medium, &value_type::small, &value_type::thumb,
                  &value_type::large>();
};

template <> struct jsonifier::core<twitter_full::media_data> {
  using value_type = twitter_full::media_data;
  static constexpr auto parseValue =
      createValue<&value_type::id, &value_type::id_str, &value_type::indices,
                  &value_type::media_url, &value_type::media_url_https,
                  &value_type::url, &value_type::display_url,
                  &value_type::expanded_url, &value_type::type,
                  &value_type::sizes, &value_type::source_status_id,
                  &value_type::source_status_id_str>();
};

template <> struct jsonifier::core<twitter_full::url_data> {
  using value_type = twitter_full::url_data;
  static constexpr auto parseValue =
      createValue<&value_type::url, &value_type::expanded_url,
                  &value_type::display_url, &value_type::indices>();
};

template <> struct jsonifier::core<twitter_full::user_mention_data> {
  using value_type = twitter_full::user_mention_data;
  static constexpr auto parseValue =
      createValue<&value_type::screen_name, &value_type::name, &value_type::id,
                  &value_type::id_str, &value_type::indices>();
};

template <> struct jsonifier::core<twitter_full::status_entities> {
  using value_type = twitter_full::status_entities;
  static constexpr auto parseValue =
      createValue<&value_type::hashtags, &value_type::symbols,
                  &value_type::urls, &value_type::user_mentions,
                  &value_type::media>();
};

template <> struct jsonifier::core<twitter_full::metadata_data> {
  using value_type = twitter_full::metadata_data;
  static constexpr auto parseValue =
      createValue<&value_type::result_type, &value_type::iso_language_code>();
};

template <> struct jsonifier::core<twitter_full::description_data> {
  using value_type = twitter_full::description_data;
  static constexpr auto parseValue = createValue<&value_type::urls>();
};

template <> struct jsonifier::core<twitter_full::user_entities> {
  using value_type = twitter_full::user_entities;
  static constexpr auto parseValue =
      createValue<&value_type::description, &value_type::url>();
};

template <> struct jsonifier::core<twitter_full::twitter_user_data> {
  using value_type = twitter_full::twitter_user_data;
  static constexpr auto parseValue = createValue<
      &value_type::id, &value_type::id_str, &value_type::name,
      &value_type::screen_name, &value_type::location, &value_type::description,
      &value_type::url, &value_type::entities,
      makeJsonEntity<&value_type::protectedVal, "protected">(),
      &value_type::followers_count, &value_type::friends_count,
      &value_type::listed_count, &value_type::created_at,
      &value_type::favourites_count, &value_type::utc_offset,
      &value_type::time_zone, &value_type::geo_enabled, &value_type::verified,
      &value_type::statuses_count, &value_type::lang,
      &value_type::contributors_enabled, &value_type::is_translator,
      &value_type::is_translation_enabled,
      &value_type::profile_background_color,
      &value_type::profile_background_image_url,
      &value_type::profile_background_image_url_https,
      &value_type::profile_background_tile, &value_type::profile_image_url,
      &value_type::profile_image_url_https, &value_type::profile_banner_url,
      &value_type::profile_link_color,
      &value_type::profile_sidebar_border_color,
      &value_type::profile_sidebar_fill_color, &value_type::profile_text_color,
      &value_type::profile_use_background_image, &value_type::default_profile,
      &value_type::default_profile_image, &value_type::following,
      &value_type::follow_request_sent, &value_type::notifications>();
};

template <> struct jsonifier::core<twitter_full::status_data> {
  using value_type = twitter_full::status_data;
  static constexpr auto parseValue = createValue<
      &value_type::metadata, &value_type::created_at, &value_type::id,
      &value_type::id_str, &value_type::text, &value_type::source,
      &value_type::truncated, &value_type::in_reply_to_status_id,
      &value_type::in_reply_to_status_id_str, &value_type::in_reply_to_user_id,
      &value_type::in_reply_to_user_id_str,
      &value_type::in_reply_to_screen_name, &value_type::user, &value_type::geo,
      &value_type::coordinates, &value_type::place, &value_type::contributors,
      &value_type::retweet_count, &value_type::favorite_count,
      &value_type::entities, &value_type::favorited, &value_type::retweeted,
      &value_type::lang, &value_type::retweeted_status,
      &value_type::possibly_sensitive>();
};

template <> struct jsonifier::core<twitter_full::twitter_message> {
  using value_type = twitter_full::twitter_message;
  static constexpr auto parseValue =
      createValue<&value_type::statuses, &value_type::search_metadata>();
};

namespace twitter_full {

struct Jsonifier {
  using StringType = std::string;

  jsonifier::jsonifier_core<> parser{};

  bool run(const std::string_view &json, int64_t max_retweet_count,
           twitter_full_result<std::string> &result) {
    return parser.parseJson<jsonifier::parse_options{
        .partialRead = true, .knownOrder = true}>(result, json);
  }
};

BENCHMARK_TEMPLATE(twitter_full, Jsonifier)->UseManualTime();

struct Jsonifier_forward_only {
  using StringType = std::string;

  jsonifier::jsonifier_core<> parser{};
  bool run(const std::string_view &json, int64_t max_retweet_count,
           twitter_full_result<std::string> &result) {
    return parser.parseJson<jsonifier::parse_options{
        .partialRead = true, .knownOrder = true}>(result, json);
  }
};

BENCHMARK_TEMPLATE(twitter_full, Jsonifier_forward_only)->UseManualTime();
} // namespace twitter_full

#endif // SIMDJSON_COMPETITION_JSONIFIER
