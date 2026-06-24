
#pragma once

#include "json_benchmark/file_runner.h"

namespace twitter_full {

  struct search_metadata_data {
  double completed_in{};
  double max_id{};
  std::string max_id_str{};
  std::string next_results{};
  std::string query{};
  std::string refresh_url{};
  int64_t count{};
  int64_t since_id{};
  std::string since_id_str{};
};

struct hashtag_data {
  std::string text{};
  std::vector<int64_t> indices{};
};

struct large_data {
  int64_t w{};
  int64_t h{};
  std::string resize{};
};

struct sizes_data {
  large_data medium{};
  large_data small{};
  large_data thumb{};
  large_data large{};
};

struct media_data {
  double id{};
  std::string id_str{};
  std::vector<int64_t> indices{};
  std::string media_url{};
  std::string media_url_https{};
  std::string url{};
  std::string display_url{};
  std::string expanded_url{};
  std::string type{};
  sizes_data sizes{};
  std::optional<double> source_status_id{};
  std::optional<std::string> source_status_id_str{};
};

struct url_data {
  std::string url{};
  std::string expanded_url{};
  std::string display_url{};
  std::vector<int64_t> indices{};
};

struct user_mention_data {
  std::string screen_name{};
  std::string name{};
  int64_t id{};
  std::string id_str{};
  std::vector<int64_t> indices{};
};

struct status_entities {
  std::vector<hashtag_data> hashtags{};
  std::vector<std::nullptr_t> symbols{};
  std::vector<url_data> urls{};
  std::vector<user_mention_data> user_mentions{};
  std::optional<std::vector<media_data>> media{};
};

struct metadata_data {
  std::string result_type{};
  std::string iso_language_code{};
};

struct description_data {
  std::vector<url_data> urls{};
};

struct user_entities {
  description_data description{};
  std::optional<description_data> url{};
};

struct twitter_user_data {
  int64_t id{};
  std::string id_str{};
  std::string name{};
  std::string screen_name{};
  std::string location{};
  std::string description{};
  std::optional<std::string> url{};
  user_entities entities{};
  bool protectedVal{};
  int64_t followers_count{};
  int64_t friends_count{};
  int64_t listed_count{};
  std::string created_at{};
  int64_t favourites_count{};
  std::optional<int64_t> utc_offset{};
  std::optional<std::string> time_zone{};
  bool geo_enabled{};
  bool verified{};
  int64_t statuses_count{};
  std::string lang{};
  bool contributors_enabled{};
  bool is_translator{};
  bool is_translation_enabled{};
  std::string profile_background_color{};
  std::string profile_background_image_url{};
  std::string profile_background_image_url_https{};
  bool profile_background_tile{};
  std::string profile_image_url{};
  std::string profile_image_url_https{};
  std::optional<std::string> profile_banner_url{};
  std::string profile_link_color{};
  std::string profile_sidebar_border_color{};
  std::string profile_sidebar_fill_color{};
  std::string profile_text_color{};
  bool profile_use_background_image{};
  bool default_profile{};
  bool default_profile_image{};
  bool following{};
  bool follow_request_sent{};
  bool notifications{};
};

struct status_data {
  metadata_data metadata{};
  std::string created_at{};
  double id{};
  std::string id_str{};
  std::string text{};
  std::string source{};
  bool truncated{};
  std::optional<double> in_reply_to_status_id{};
  std::optional<std::string> in_reply_to_status_id_str{};
  std::optional<int64_t> in_reply_to_user_id{};
  std::optional<std::string> in_reply_to_user_id_str{};
  std::optional<std::string> in_reply_to_screen_name{};
  twitter_user_data user{};
  std::nullptr_t geo{};
  std::nullptr_t coordinates{};
  std::nullptr_t place{};
  std::nullptr_t contributors{};
  int64_t retweet_count{};
  int64_t favorite_count{};
  status_entities entities{};
  bool favorited{};
  bool retweeted{};
  std::string lang{};
  std::optional<bool> possibly_sensitive{};
};

struct twitter_message {
  std::vector<status_data> statuses{};
  search_metadata_data search_metadata{};
};

using namespace json_benchmark;

    inline bool
    operator==(const large_data &a, const large_data &b) {
  return a.w == b.w && a.h == b.h && a.resize == b.resize;
}

inline bool operator==(const sizes_data &a, const sizes_data &b) {
  return a.medium == b.medium && a.small == b.small && a.thumb == b.thumb &&
         a.large == b.large;
}

inline bool operator==(const hashtag_data &a, const hashtag_data &b) {
  return a.text == b.text && a.indices == b.indices;
}

inline bool operator==(const url_data &a, const url_data &b) {
  return a.url == b.url && a.expanded_url == b.expanded_url &&
         a.display_url == b.display_url && a.indices == b.indices;
}

inline bool operator==(const user_mention_data &a, const user_mention_data &b) {
  return a.screen_name == b.screen_name && a.name == b.name && a.id == b.id &&
         a.id_str == b.id_str && a.indices == b.indices;
}

inline bool operator==(const media_data &a, const media_data &b) {
  return a.id == b.id && a.id_str == b.id_str && a.indices == b.indices &&
         a.media_url == b.media_url && a.media_url_https == b.media_url_https &&
         a.url == b.url && a.display_url == b.display_url &&
         a.expanded_url == b.expanded_url && a.type == b.type &&
         a.sizes == b.sizes && a.source_status_id == b.source_status_id &&
         a.source_status_id_str == b.source_status_id_str;
}

inline bool operator==(const status_entities &a, const status_entities &b) {
  return a.hashtags == b.hashtags && a.symbols.size() == b.symbols.size() &&
         a.urls == b.urls && a.user_mentions == b.user_mentions &&
         a.media == b.media;
}

inline bool operator==(const metadata_data &a, const metadata_data &b) {
  return a.result_type == b.result_type &&
         a.iso_language_code == b.iso_language_code;
}

inline bool operator==(const description_data &a, const description_data &b) {
  return a.urls == b.urls;
}

inline bool operator==(const user_entities &a, const user_entities &b) {
  return a.description == b.description && a.url == b.url;
}

inline bool operator==(const twitter_user_data &a, const twitter_user_data &b) {
  return a.id == b.id && a.id_str == b.id_str && a.name == b.name &&
         a.screen_name == b.screen_name && a.location == b.location &&
         a.description == b.description && a.url == b.url &&
         a.entities == b.entities && a.protectedVal == b.protectedVal &&
         a.followers_count == b.followers_count &&
         a.friends_count == b.friends_count &&
         a.listed_count == b.listed_count && a.created_at == b.created_at &&
         a.favourites_count == b.favourites_count &&
         a.utc_offset == b.utc_offset && a.time_zone == b.time_zone &&
         a.geo_enabled == b.geo_enabled && a.verified == b.verified &&
         a.statuses_count == b.statuses_count && a.lang == b.lang &&
         a.contributors_enabled == b.contributors_enabled &&
         a.is_translator == b.is_translator &&
         a.is_translation_enabled == b.is_translation_enabled &&
         a.profile_background_color == b.profile_background_color &&
         a.profile_background_image_url == b.profile_background_image_url &&
         a.profile_background_image_url_https ==
             b.profile_background_image_url_https &&
         a.profile_background_tile == b.profile_background_tile &&
         a.profile_image_url == b.profile_image_url &&
         a.profile_image_url_https == b.profile_image_url_https &&
         a.profile_banner_url == b.profile_banner_url &&
         a.profile_link_color == b.profile_link_color &&
         a.profile_sidebar_border_color == b.profile_sidebar_border_color &&
         a.profile_sidebar_fill_color == b.profile_sidebar_fill_color &&
         a.profile_text_color == b.profile_text_color &&
         a.profile_use_background_image == b.profile_use_background_image &&
         a.default_profile == b.default_profile &&
         a.default_profile_image == b.default_profile_image &&
         a.following == b.following &&
         a.follow_request_sent == b.follow_request_sent &&
         a.notifications == b.notifications;
}

inline bool operator==(const search_metadata_data &a,
                       const search_metadata_data &b) {
  return a.completed_in == b.completed_in && a.max_id == b.max_id &&
         a.max_id_str == b.max_id_str && a.next_results == b.next_results &&
         a.query == b.query && a.refresh_url == b.refresh_url &&
         a.count == b.count && a.since_id == b.since_id &&
         a.since_id_str == b.since_id_str;
}

inline bool operator==(const status_data &a, const status_data &b) {
  return a.metadata == b.metadata && a.created_at == b.created_at &&
         a.id == b.id && a.id_str == b.id_str && a.text == b.text &&
         a.source == b.source && a.truncated == b.truncated &&
         a.in_reply_to_status_id == b.in_reply_to_status_id &&
         a.in_reply_to_status_id_str == b.in_reply_to_status_id_str &&
         a.in_reply_to_user_id == b.in_reply_to_user_id &&
         a.in_reply_to_user_id_str == b.in_reply_to_user_id_str &&
         a.in_reply_to_screen_name == b.in_reply_to_screen_name &&
         a.user == b.user && a.retweet_count == b.retweet_count &&
         a.favorite_count == b.favorite_count && a.entities == b.entities &&
         a.favorited == b.favorited && a.retweeted == b.retweeted &&
         a.lang == b.lang && a.possibly_sensitive == b.possibly_sensitive;
}

inline bool operator==(const twitter_message &a, const twitter_message &b) {
  return a.statuses == b.statuses && a.search_metadata == b.search_metadata;
}

inline std::ostream &operator<<(std::ostream &o, const large_data &v) {
  return o << "w: " << v.w << " h: " << v.h << " resize: " << v.resize;
}

inline std::ostream &operator<<(std::ostream &o, const sizes_data &v) {
  return o << "medium: {" << v.medium << "} small: {" << v.small << "} thumb: {"
           << v.thumb << "} large: {" << v.large << "}";
}

inline std::ostream &operator<<(std::ostream &o, const hashtag_data &v) {
  o << "text: " << v.text << " indices: [";
  for (size_t i = 0; i < v.indices.size(); ++i) {
    if (i)
      o << ", ";
    o << v.indices[i];
  }
  return o << "]";
}

inline std::ostream &operator<<(std::ostream &o, const url_data &v) {
  return o << "url: " << v.url << " expanded_url: " << v.expanded_url
           << " display_url: " << v.display_url;
}

inline std::ostream &operator<<(std::ostream &o, const user_mention_data &v) {
  return o << "screen_name: " << v.screen_name << " name: " << v.name
           << " id: " << v.id;
}

inline std::ostream &operator<<(std::ostream &o, const media_data &v) {
  return o << "id: " << v.id << " url: " << v.media_url << " type: " << v.type;
}

inline std::ostream &operator<<(std::ostream &o, const status_entities &v) {
  o << "hashtags: " << v.hashtags.size() << " urls: " << v.urls.size()
    << " user_mentions: " << v.user_mentions.size()
    << " symbols: " << v.symbols.size();
  if (v.media)
    o << " media: " << v.media->size();
  return o;
}

inline std::ostream &operator<<(std::ostream &o, const metadata_data &v) {
  return o << "result_type: " << v.result_type
           << " iso_language_code: " << v.iso_language_code;
}

inline std::ostream &operator<<(std::ostream &o, const description_data &v) {
  return o << "urls: " << v.urls.size();
}

inline std::ostream &operator<<(std::ostream &o, const user_entities &v) {
  o << "description: {" << v.description << "}";
  if (v.url)
    o << " url: {" << *v.url << "}";
  return o;
}

inline std::ostream &operator<<(std::ostream &o, const twitter_user_data &v) {
  return o << "id: " << v.id << " screen_name: " << v.screen_name
           << " name: " << v.name << " followers: " << v.followers_count
           << " friends: " << v.friends_count
           << " statuses: " << v.statuses_count << " verified: " << v.verified;
}

inline std::ostream &operator<<(std::ostream &o,
                                const search_metadata_data &v) {
  return o << "completed_in: " << v.completed_in << " count: " << v.count
           << " max_id_str: " << v.max_id_str << " query: " << v.query;
}

inline std::ostream &operator<<(std::ostream &o, const status_data &v) {
  return o << "id: " << v.id << " text: " << v.text << " user: {" << v.user
           << "} retweet_count: " << v.retweet_count
           << " favorite_count: " << v.favorite_count << " lang: " << v.lang
           << " entities: {" << v.entities << "}";
}

inline std::ostream &operator<<(std::ostream &o, const twitter_message &v) {
  o << "statuses: " << v.statuses.size() << std::endl;
  for (const auto &s : v.statuses) {
    o << "  " << s << std::endl;
  }
  o << "search_metadata: {" << v.search_metadata << "}" << std::endl;
  return o;
}

template <typename StringType> struct twitter_full_result {
  twitter_message message{};
  template <typename OtherStringType>
  simdjson_inline bool
  operator==(const twitter_full_result<OtherStringType> &other) const {
    return message == other.message;
  }
  template <typename OtherStringType>
  simdjson_inline bool
  operator!=(const twitter_full_result<OtherStringType> &other) const {
    return !(*this == other);
  }
};

template <typename StringType>
simdjson_unused static std::ostream &
operator<<(std::ostream &o, const twitter_full_result<StringType> &t) {
  return o << t.message;
}

template <typename I> struct runner : public file_runner<I> {

  twitter_full_result<typename I::StringType> result{};
  bool setup(benchmark::State &state) {

    return this->load_json(state, TWITTER_JSON);
  }
  bool before_run(benchmark::State &state) {

    if (!file_runner<I>::before_run(state)) {

      return false;
    }

    result.message.statuses.clear();

    return true;
  }
  bool run(benchmark::State &) {

    return this->implementation.run(this->json, result);
  }
  template <typename R>

  bool diff(benchmark::State &state, runner<R> &reference) {

    return diff_results(state, result, reference.result, diff_flags::NONE);
  }
  size_t items_per_iteration() { return result.message.statuses.size(); }
};
template <typename I>

simdjson_inline static void twitter_full(benchmark::State &state) {

  json_benchmark::run_json_benchmark<runner<I>, runner<I>>(state);
}
} // namespace twitter_full
