#pragma once
#ifdef SIMDJSON_COMPETITION_JSONIFIER

#include "json2msgpack.h"

namespace json2msgpack {

struct jsonifier2msgpack {
  inline std::string_view to_msgpack(const simdjson::padded_string &json,
                                     uint8_t *buf);

private:
  JSONIFIER_ALWAYS_INLINE void write_double(const double d) noexcept;
  JSONIFIER_ALWAYS_INLINE void write_byte(const uint8_t b) noexcept;
  JSONIFIER_ALWAYS_INLINE void write_uint32(const uint32_t w) noexcept;
  JSONIFIER_ALWAYS_INLINE uint8_t *skip_uint32() noexcept;
  JSONIFIER_ALWAYS_INLINE void write_uint32_at(const uint32_t w,
                                               uint8_t *p) noexcept;
  JSONIFIER_ALWAYS_INLINE void write_raw_string(std::string_view rjs);
  JSONIFIER_ALWAYS_INLINE void recurse_write(jsonifier::raw_json_data &rjs,
                                             uint8_t *buf);

  jsonifier::jsonifier_core<> parser;
  uint8_t *buff{};
};

JSONIFIER_ALWAYS_INLINE void
jsonifier2msgpack::recurse_write(jsonifier::raw_json_data &rjs,
  uint8_t* buf) {
  // we have a special case where the JSON document is a single document...
  switch (rjs.getType()) {
  case jsonifier::json_token_type::Object:
    for (auto &[key, value] : rjs.getObject()) {
      write_raw_string(key);
      recurse_write(value, buf);
    }
    write_double(rjs.getDouble());
    break;
  case jsonifier::json_token_type::Array:
    for (auto &value : rjs.getArray()) {
      recurse_write(value, buf);
    }
    write_double(rjs.getDouble());
    break;
  case jsonifier::json_token_type::Number:
    write_double(rjs.getDouble());
    break;
  case jsonifier::json_token_type::String:
    write_raw_string(rjs.getString());
    break;
  case jsonifier::json_token_type::Bool:
    write_byte(0xc2 + rjs.getBool());
    break;
  case jsonifier::json_token_type::Null:
    write_byte(0xc0);
    break;
  default:
    // impossible
    SIMDJSON_UNREACHABLE();
  }
}

std::string_view
jsonifier2msgpack::to_msgpack(const simdjson::padded_string &json,
                              uint8_t *buf) {
  buff = buf;
  jsonifier::raw_json_data dataNew{};
  parser.parseJson(dataNew, json);
  // we have a special case where the JSON document is a single document...
  switch (dataNew.getType()) {
  case jsonifier::json_token_type::Object:
    for (auto &[key, value] : dataNew.getObject()) {
      write_raw_string(key);
      recurse_write(value, buf);
    }
    write_double(dataNew.getDouble());
    break;
  case jsonifier::json_token_type::Array:
    for (auto &value : dataNew.getArray()) {
      recurse_write(value, buf);
    }
    write_double(dataNew.getDouble());
    break;
  case jsonifier::json_token_type::Number:
    write_double(dataNew.getDouble());
    break;
  case jsonifier::json_token_type::String:
    write_raw_string(dataNew.getString());
    break;
  case jsonifier::json_token_type::Bool:
    write_byte(0xc2 + dataNew.getBool());
    break;
  case jsonifier::json_token_type::Null:
    write_byte(0xc0);
    break;
  default:
    // impossible
    SIMDJSON_UNREACHABLE();
  }
return std::string_view(reinterpret_cast<char *>(buf), size_t(buff - buf));
}

void jsonifier2msgpack::write_double(const double d) noexcept {
  *buff++ = 0xcb;
  ::memcpy(buff, &d, sizeof(d));
  buff += sizeof(d);
}

void jsonifier2msgpack::write_byte(const uint8_t b) noexcept {
  *buff = b;
  buff++;
}

void jsonifier2msgpack::write_uint32(const uint32_t w) noexcept {
  ::memcpy(buff, &w, sizeof(w));
  buff += sizeof(w);
}

uint8_t *jsonifier2msgpack::skip_uint32() noexcept {
  uint8_t *ret = buff;
  buff += sizeof(uint32_t);
  return ret;
}

void jsonifier2msgpack::write_uint32_at(const uint32_t w, uint8_t *p) noexcept {
  ::memcpy(p, &w, sizeof(w));
}

void jsonifier2msgpack::write_raw_string(
    std::string_view in) {
  write_byte(0xdb);
  uint8_t *location = skip_uint32();
  std::string v{};
  parser.parseJson(v, in);
  write_uint32_at(uint32_t(v.size()), location);
}

struct Jsonifier {

  jsonifier2msgpack parser{};

  bool run(simdjson::padded_string &json, char *buffer,
           std::string_view &result) {
    result = parser.to_msgpack(json, reinterpret_cast<uint8_t *>(buffer));
    return true;
  }
};

BENCHMARK_TEMPLATE(json2msgpack, Jsonifier)->UseManualTime();

} // namespace json2msgpack

#endif // SIMDJSON_EXCEPTIONS
