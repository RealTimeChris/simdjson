#pragma once
#if SIMDJSON2_EXCEPTIONS


#include "json2msgpack.h"

namespace json2msgpack {

using namespace simdjson2;

struct simdjson2dom2msgpack {
  /**
   * @brief Converts the provided JSON into msgpack.
   *
   * @param json JSON input
   * @param buf temporary buffer (must be large enough, with simdjson2::SIMDJSON2_PADDING bytes
   * of padding)
   * @return std::string_view msgpack output, writing to the temporary buffer
   */
  inline std::string_view to_msgpack(const simdjson2::padded_string &json,
                                     uint8_t *buf);

private:
  simdjson2_really_inline void write_double(const double d) noexcept;
  simdjson2_really_inline void write_string(const std::string_view v) noexcept;
  simdjson2_really_inline void write_byte(const uint8_t b) noexcept;
  simdjson2_really_inline void write_uint32(const uint32_t w) noexcept;
  simdjson2_really_inline uint8_t *skip_uint32() noexcept;
  simdjson2_really_inline void write_uint32_at(const uint32_t w,
                                              uint8_t *p) noexcept;

  inline void recursive_processor(simdjson2::dom::element element);

  dom::parser parser;
  uint8_t *buff{};
};

std::string_view
simdjson2dom2msgpack::to_msgpack(const simdjson2::padded_string &json,
                             uint8_t *buf) {
  buff = buf;

  recursive_processor(parser.parse(json));
  return std::string_view(reinterpret_cast<char *>(buf), size_t(buff - buf));
}
void simdjson2dom2msgpack::write_string(const std::string_view v) noexcept {
  write_byte(0xdb);
  write_uint32(uint32_t(v.size()));
  ::memcpy(buff, v.data(), v.size());
  buff += v.size();
}
void simdjson2dom2msgpack::write_double(const double d) noexcept {
  *buff++ = 0xcb;
  ::memcpy(buff, &d, sizeof(d));
  buff += sizeof(d);
}

void simdjson2dom2msgpack::write_byte(const uint8_t b) noexcept {
  *buff = b;
  buff++;
}

void simdjson2dom2msgpack::write_uint32(const uint32_t w) noexcept {
  ::memcpy(buff, &w, sizeof(w));
  buff += sizeof(w);
}

uint8_t *simdjson2dom2msgpack::skip_uint32() noexcept {
  uint8_t *ret = buff;
  buff += sizeof(uint32_t);
  return ret;
}

void simdjson2dom2msgpack::write_uint32_at(const uint32_t w, uint8_t *p) noexcept {
  ::memcpy(p, &w, sizeof(w));
}


void simdjson2dom2msgpack::recursive_processor(simdjson2::dom::element element) {
  switch (element.type()) {
    case dom::element_type::ARRAY: {
      uint32_t counter = 0;
      write_byte(0xdd);
      uint8_t *location = skip_uint32();
      for (auto child : dom::array(element)) {
        counter++;
        recursive_processor(child);
      }
      write_uint32_at(counter, location);}
      break;
    case dom::element_type::OBJECT:{
      uint32_t counter = 0;
      write_byte(0xdf);
      uint8_t *location = skip_uint32();
      for (dom::key_value_pair field : dom::object(element)) {
        counter++;
        write_string(field.key);
        recursive_processor(field.value);
      }
      write_uint32_at(counter, location);
      }
      break;
    case dom::element_type::INT64:
    case dom::element_type::UINT64:
    case dom::element_type::DOUBLE:
      write_double( double(element));
      break;
    case dom::element_type::STRING:
      write_string(std::string_view(element));
      break;
    case dom::element_type::BOOL:
      write_byte(0xc2 + bool(element));
      break;
    case dom::element_type::NULL_VALUE:
      write_byte(0xc0);
      break;
    default:
      break;
  }
}

struct simdjson2_dom {
  using StringType = std::string_view;

  simdjson2dom2msgpack parser{};

  bool run(simdjson2::padded_string &json, char *buffer,
           std::string_view &result) {
    result = parser.to_msgpack(json, reinterpret_cast<uint8_t *>(buffer));
    return true;
  }
};

BENCHMARK_TEMPLATE(json2msgpack, simdjson2_dom)->UseManualTime();

} // namespace json2msgpack

#endif // SIMDJSON2_EXCEPTIONS