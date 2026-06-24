#pragma once

#if SIMDJSON_EXCEPTIONS

#include "twitter_full.h"

namespace twitter_full {
using namespace simdjson;

struct simdjson_ondemand {
  using StringType = std::string_view;
  ondemand::parser parser{};

  bool parse_large(ondemand::object &obj, large_data &out) {
    out.w = int64_t(obj["w"]);
    out.h = int64_t(obj["h"]);
    out.resize = std::string_view(obj["resize"]);
    return true;
  }

  bool parse_sizes(ondemand::object &obj, sizes_data &out) {
    auto medium = obj["medium"].get_object();
    parse_large(medium.value(), out.medium);
    auto small_ = obj["small"].get_object();
    parse_large(small_.value(), out.small);
    auto thumb = obj["thumb"].get_object();
    parse_large(thumb.value(), out.thumb);
    auto large_ = obj["large"].get_object();
    parse_large(large_.value(), out.large);
    return true;
  }

  bool parse_url_data(ondemand::object &obj, url_data &out) {
    out.url = std::string_view(obj["url"]);
    out.expanded_url = std::string_view(obj["expanded_url"]);
    out.display_url = std::string_view(obj["display_url"]);
    for (auto idx : obj["indices"].get_array()) {
      out.indices.push_back(int64_t(idx));
    }
    return true;
  }

  bool parse_hashtag(ondemand::object &obj, hashtag_data &out) {
    out.text = std::string_view(obj["text"]);
    for (auto idx : obj["indices"].get_array()) {
      out.indices.push_back(int64_t(idx));
    }
    return true;
  }

  bool parse_user_mention(ondemand::object &obj, user_mention_data &out) {
    out.screen_name = std::string_view(obj["screen_name"]);
    out.name = std::string_view(obj["name"]);
    out.id = int64_t(obj["id"]);
    out.id_str = std::string_view(obj["id_str"]);
    for (auto idx : obj["indices"].get_array()) {
      out.indices.push_back(int64_t(idx));
    }
    return true;
  }

  bool parse_media(ondemand::object &obj, media_data &out) {
    out.id = double(obj["id"]);
    out.id_str = std::string_view(obj["id_str"]);
    for (auto idx : obj["indices"].get_array()) {
      out.indices.push_back(int64_t(idx));
    }
    out.media_url = std::string_view(obj["media_url"]);
    out.media_url_https = std::string_view(obj["media_url_https"]);
    out.url = std::string_view(obj["url"]);
    out.display_url = std::string_view(obj["display_url"]);
    out.expanded_url = std::string_view(obj["expanded_url"]);
    out.type = std::string_view(obj["type"]);
    auto sizes_obj = obj["sizes"].get_object();
    parse_sizes(sizes_obj.value(), out.sizes);
    ondemand::value source_id;
    if (!obj["source_status_id"].get(source_id)) {
      if (source_id.type() != ondemand::json_type::null) {
        out.source_status_id = double(source_id);
      }
    }
    ondemand::value source_id_str;
    if (!obj["source_status_id_str"].get(source_id_str)) {
      if (source_id_str.type() != ondemand::json_type::null) {
        out.source_status_id_str = std::string_view(source_id_str);
      }
    }
    return true;
  }

  bool parse_status_entities(ondemand::object &obj, status_entities &out) {
    for (auto tag : obj["hashtags"].get_array()) {
      hashtag_data h{};
      auto tag_obj = tag.get_object();
      parse_hashtag(tag_obj.value(), h);
      out.hashtags.push_back(std::move(h));
    }
    for (auto sym : obj["symbols"].get_array()) {
      if (sym.type() != ondemand::json_type::null) {
        return false;
      }
      out.symbols.push_back(nullptr);
    }
    for (auto u : obj["urls"].get_array()) {
      url_data ud{};
      auto u_obj = u.get_object();
      parse_url_data(u_obj.value(), ud);
      out.urls.push_back(std::move(ud));
    }
    for (auto um : obj["user_mentions"].get_array()) {
      user_mention_data umd{};
      auto um_obj = um.get_object();
      parse_user_mention(um_obj.value(), umd);
      out.user_mentions.push_back(std::move(umd));
    }
    ondemand::value media_val;
    if (!obj["media"].get(media_val)) {
      if (media_val.type() != ondemand::json_type::null) {
        out.media.emplace();
        for (auto m : media_val.get_array()) {
          media_data md{};
          auto m_obj = m.get_object();
          parse_media(m_obj.value(), md);
          out.media->push_back(std::move(md));
        }
      }
    }
    return true;
  }

  bool parse_description(ondemand::object &obj, description_data &out) {
    for (auto u : obj["urls"].get_array()) {
      url_data ud{};
      auto u_obj = u.get_object();
      parse_url_data(u_obj.value(), ud);
      out.urls.push_back(std::move(ud));
    }
    return true;
  }

  bool parse_user_entities(ondemand::object &obj, user_entities &out) {
    auto desc_obj = obj["description"].get_object();
    parse_description(desc_obj.value(), out.description);
    ondemand::value url_val;
    if (!obj["url"].get(url_val)) {
      if (url_val.type() != ondemand::json_type::null) {
        out.url.emplace();
        auto url_obj = url_val.get_object();
        parse_description(url_obj.value(), out.url.value());
      }
    }
    return true;
  }

  bool parse_user(ondemand::object &obj, twitter_user_data &out) {
    out.id = int64_t(obj["id"]);
    out.id_str = std::string_view(obj["id_str"]);
    out.name = std::string_view(obj["name"]);
    out.screen_name = std::string_view(obj["screen_name"]);
    out.location = std::string_view(obj["location"]);
    out.description = std::string_view(obj["description"]);
    ondemand::value url_val;
    if (!obj["url"].get(url_val)) {
      if (url_val.type() != ondemand::json_type::null) {
        out.url = std::string_view(url_val);
      }
    }
    auto ent_obj = obj["entities"].get_object();
    parse_user_entities(ent_obj.value(), out.entities);
    out.protectedVal = bool(obj["protected"]);
    out.followers_count = int64_t(obj["followers_count"]);
    out.friends_count = int64_t(obj["friends_count"]);
    out.listed_count = int64_t(obj["listed_count"]);
    out.created_at = std::string_view(obj["created_at"]);
    out.favourites_count = int64_t(obj["favourites_count"]);
    ondemand::value utc_val;
    if (!obj["utc_offset"].get(utc_val)) {
      if (utc_val.type() != ondemand::json_type::null) {
        out.utc_offset = int64_t(utc_val);
      }
    }
    ondemand::value tz_val;
    if (!obj["time_zone"].get(tz_val)) {
      if (tz_val.type() != ondemand::json_type::null) {
        out.time_zone = std::string_view(tz_val);
      }
    }
    out.geo_enabled = bool(obj["geo_enabled"]);
    out.verified = bool(obj["verified"]);
    out.statuses_count = int64_t(obj["statuses_count"]);
    out.lang = std::string_view(obj["lang"]);
    out.contributors_enabled = bool(obj["contributors_enabled"]);
    out.is_translator = bool(obj["is_translator"]);
    out.is_translation_enabled = bool(obj["is_translation_enabled"]);
    out.profile_background_color =
        std::string_view(obj["profile_background_color"]);
    out.profile_background_image_url =
        std::string_view(obj["profile_background_image_url"]);
    out.profile_background_image_url_https =
        std::string_view(obj["profile_background_image_url_https"]);
    out.profile_background_tile = bool(obj["profile_background_tile"]);
    out.profile_image_url = std::string_view(obj["profile_image_url"]);
    out.profile_image_url_https =
        std::string_view(obj["profile_image_url_https"]);
    ondemand::value banner_val;
    if (!obj["profile_banner_url"].get(banner_val)) {
      if (banner_val.type() != ondemand::json_type::null) {
        out.profile_banner_url = std::string_view(banner_val);
      }
    }
    out.profile_link_color = std::string_view(obj["profile_link_color"]);
    out.profile_sidebar_border_color =
        std::string_view(obj["profile_sidebar_border_color"]);
    out.profile_sidebar_fill_color =
        std::string_view(obj["profile_sidebar_fill_color"]);
    out.profile_text_color = std::string_view(obj["profile_text_color"]);
    out.profile_use_background_image =
        bool(obj["profile_use_background_image"]);
    out.default_profile = bool(obj["default_profile"]);
    out.default_profile_image = bool(obj["default_profile_image"]);
    out.following = bool(obj["following"]);
    out.follow_request_sent = bool(obj["follow_request_sent"]);
    out.notifications = bool(obj["notifications"]);
    return true;
  }

  bool parse_metadata(ondemand::object &obj, metadata_data &out) {
    out.result_type = std::string_view(obj["result_type"]);
    out.iso_language_code = std::string_view(obj["iso_language_code"]);
    return true;
  }

  bool parse_status(ondemand::object &obj, status_data &out) {
    auto meta_obj = obj["metadata"].get_object();
    parse_metadata(meta_obj.value(), out.metadata);
    out.created_at = std::string_view(obj["created_at"]);
    out.id = double(obj["id"]);
    out.id_str = std::string_view(obj["id_str"]);
    out.text = std::string_view(obj["text"]);
    out.source = std::string_view(obj["source"]);
    out.truncated = bool(obj["truncated"]);
    ondemand::value reply_id;
    if (!obj["in_reply_to_status_id"].get(reply_id)) {
      if (reply_id.type() != ondemand::json_type::null) {
        out.in_reply_to_status_id = double(reply_id);
      }
    }
    ondemand::value reply_id_str;
    if (!obj["in_reply_to_status_id_str"].get(reply_id_str)) {
      if (reply_id_str.type() != ondemand::json_type::null) {
        out.in_reply_to_status_id_str = std::string_view(reply_id_str);
      }
    }
    ondemand::value reply_user_id;
    if (!obj["in_reply_to_user_id"].get(reply_user_id)) {
      if (reply_user_id.type() != ondemand::json_type::null) {
        out.in_reply_to_user_id = int64_t(reply_user_id);
      }
    }
    ondemand::value reply_user_id_str;
    if (!obj["in_reply_to_user_id_str"].get(reply_user_id_str)) {
      if (reply_user_id_str.type() != ondemand::json_type::null) {
        out.in_reply_to_user_id_str = std::string_view(reply_user_id_str);
      }
    }
    ondemand::value reply_screen_name;
    if (!obj["in_reply_to_screen_name"].get(reply_screen_name)) {
      if (reply_screen_name.type() != ondemand::json_type::null) {
        out.in_reply_to_screen_name = std::string_view(reply_screen_name);
      }
    }
    auto user_obj = obj["user"].get_object();
    parse_user(user_obj.value(), out.user);
    if (obj["geo"].type() != ondemand::json_type::null) {
      return false;
    }
    if (obj["coordinates"].type() != ondemand::json_type::null) {
      return false;
    }
    if (obj["place"].type() != ondemand::json_type::null) {
      return false;
    }
    if (obj["contributors"].type() != ondemand::json_type::null) {
      return false;
    }
    out.retweet_count = int64_t(obj["retweet_count"]);
    out.favorite_count = int64_t(obj["favorite_count"]);
    auto ent_obj = obj["entities"].get_object();
    parse_status_entities(ent_obj.value(), out.entities);
    out.favorited = bool(obj["favorited"]);
    out.retweeted = bool(obj["retweeted"]);
    out.lang = std::string_view(obj["lang"]);
    ondemand::value ps_val;
    if (!obj["possibly_sensitive"].get(ps_val)) {
      if (ps_val.type() != ondemand::json_type::null) {
        out.possibly_sensitive = bool(ps_val);
      }
    }
    return true;
  }

  bool parse_search_metadata(ondemand::object &obj, search_metadata_data &out) {
    out.completed_in = double(obj["completed_in"]);
    out.max_id = double(obj["max_id"]);
    out.max_id_str = std::string_view(obj["max_id_str"]);
    out.next_results = std::string_view(obj["next_results"]);
    out.query = std::string_view(obj["query"]);
    out.refresh_url = std::string_view(obj["refresh_url"]);
    out.count = int64_t(obj["count"]);
    out.since_id = int64_t(obj["since_id"]);
    out.since_id_str = std::string_view(obj["since_id_str"]);
    return true;
  }

  bool run(simdjson::padded_string &json,
           twitter_full_result<std::string_view> &result) {
    auto doc = parser.iterate(json);
    for (auto tweet : doc["statuses"]) {
      status_data s{};
      auto tweet_obj = tweet.get_object();
      if (!parse_status(tweet_obj.value(), s)) {
        return false;
      }
      result.message.statuses.push_back(std::move(s));
    }
    auto meta_obj = doc["search_metadata"].get_object();
    if (!parse_search_metadata(meta_obj.value(),
                               result.message.search_metadata)) {
      return false;
    }
    return true;
  }
};

BENCHMARK_TEMPLATE(twitter_full, simdjson_ondemand)->UseManualTime();
} // namespace twitter_full
#endif
