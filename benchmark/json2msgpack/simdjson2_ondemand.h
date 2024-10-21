#pragma once
#if SIMDJSON2_EXCEPTIONS

#include "json2msgpack.h"

namespace json2msgpack {

using namespace simdjson2;

/**
 * @brief The simdjson22msgpack struct is used to quickly convert
 * JSON strings to msgpack views. You must provide a pointer to
 * a large memory region where the msgpack gets written. The
 * buffer should be large enough to store the msgpack output (which
 * can never be 3x larger than the input JSON) with an additional
 * simdjson2::SIMDJSON2_PADDING bytes.
 *
 * Recommended usage:
 *
 * simdjson22msgpack parser{};
 * simdjson2::padded_string json = "[1,2]"_padded; // some JSON
 * uint8_t * buffer = new uint8_t[3*json.size() + simdjson2::SIMDJSON2_PADDING]; // large buffer
 *
 * std::string_view msgpack = parser.to_msgpack(json, buffer);
 *
 * The result (msgpack) is a string view to a msgpack serialization of the input JSON,
 * it points inside the buffer you provided.
 *
 * You may reuse the simdjson22msgpack instance though you should use
 * one per thread.
 */
struct simdjson22msgpack {
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
  simdjson2_inline void write_double(const double d) noexcept;
  simdjson2_inline void write_byte(const uint8_t b) noexcept;
  simdjson2_inline void write_uint32(const uint32_t w) noexcept;
  simdjson2_inline uint8_t *skip_uint32() noexcept;
  simdjson2_inline void write_uint32_at(const uint32_t w,
                                              uint8_t *p) noexcept;
  simdjson2_inline void
  write_raw_string(simdjson2::ondemand::raw_json_string rjs);
  inline void recursive_processor(simdjson2::ondemand::value element);
  inline void recursive_processor_ref(simdjson2::ondemand::value& element);

  simdjson2::ondemand::parser parser;
  uint8_t *buff{};
};

std::string_view
simdjson22msgpack::to_msgpack(const simdjson2::padded_string &json,
                             uint8_t *buf) {
  buff = buf;
  ondemand::document doc = parser.iterate(json);
  if (doc.is_scalar()) {
    // we have a special case where the JSON document is a single document...
    switch (doc.type()) {
    case simdjson2::ondemand::json_type::number:
      write_double(doc.get_double());
      break;
    case simdjson2::ondemand::json_type::string:
      write_raw_string(doc.get_raw_json_string());
      break;
    case simdjson2::ondemand::json_type::boolean:
      write_byte(0xc2 + doc.get_bool());
      break;
    case simdjson2::ondemand::json_type::null:
      // We check that the value is indeed null
      // otherwise: an error is thrown.
      if(doc.is_null()) {
        write_byte(0xc0);
      }
      break;
    case simdjson2::ondemand::json_type::array:
    case simdjson2::ondemand::json_type::object:
    default:
      // impossible
      SIMDJSON2_UNREACHABLE();
    }
  } else {
    simdjson2::ondemand::value val = doc;
#define SIMDJSON2_GCC_COMPILER ((__GNUC__) && !(__clang__) && !(__INTEL_COMPILER))
#if SIMDJSON2_GCC_COMPILER
    // the GCC compiler does well with by-value passing.
    // GCC has superior recursive inlining:
    // https://stackoverflow.com/questions/29186186/why-does-gcc-generate-a-faster-program-than-clang-in-this-recursive-fibonacci-co
    // https://godbolt.org/z/TeK4doE51
    recursive_processor(val);
#else
    recursive_processor_ref(val);
#endif
  }
  if (!doc.at_end()) {
     throw "There are unexpectedly tokens after the end of the json in the json2msgpack sample data";
  }
  return std::string_view(reinterpret_cast<char *>(buf), size_t(buff - buf));
}

void simdjson22msgpack::write_double(const double d) noexcept {
  *buff++ = 0xcb;
  ::memcpy(buff, &d, sizeof(d));
  buff += sizeof(d);
}

void simdjson22msgpack::write_byte(const uint8_t b) noexcept {
  *buff = b;
  buff++;
}

void simdjson22msgpack::write_uint32(const uint32_t w) noexcept {
  ::memcpy(buff, &w, sizeof(w));
  buff += sizeof(w);
}

uint8_t *simdjson22msgpack::skip_uint32() noexcept {
  uint8_t *ret = buff;
  buff += sizeof(uint32_t);
  return ret;
}

void simdjson22msgpack::write_uint32_at(const uint32_t w, uint8_t *p) noexcept {
  ::memcpy(p, &w, sizeof(w));
}

void simdjson22msgpack::write_raw_string(
    simdjson2::ondemand::raw_json_string in) {
  write_byte(0xdb);
  uint8_t *location = skip_uint32();
  std::string_view v = parser.unescape(in, buff);
  write_uint32_at(uint32_t(v.size()), location);
}

void simdjson22msgpack::recursive_processor(simdjson2::ondemand::value element) {
  switch (element.type()) {
  case simdjson2::ondemand::json_type::array: {
    uint32_t counter = 0;
    write_byte(0xdd);
    uint8_t *location = skip_uint32();
    for (auto child : element.get_array()) {
      counter++;
      recursive_processor(child.value());
    }
    write_uint32_at(counter, location);
  } break;
  case simdjson2::ondemand::json_type::object: {
    uint32_t counter = 0;
    write_byte(0xdf);
    uint8_t *location = skip_uint32();
    for (auto field : element.get_object()) {
      counter++;
      write_raw_string(field.key());
      recursive_processor(field.value());
    }
    write_uint32_at(counter, location);
  } break;
  case simdjson2::ondemand::json_type::number:
    write_double(element.get_double());
    break;
  case simdjson2::ondemand::json_type::string:
    write_raw_string(element.get_raw_json_string());
    break;
  case simdjson2::ondemand::json_type::boolean:
    write_byte(0xc2 + element.get_bool());
    break;
  case simdjson2::ondemand::json_type::null:
    // We check that the value is indeed null
    // otherwise: an error is thrown.
    if(element.is_null()) {
      write_byte(0xc0);
    }
    break;
  default:
    SIMDJSON2_UNREACHABLE();
  }
}


void simdjson22msgpack::recursive_processor_ref(simdjson2::ondemand::value& element) {
  switch (element.type()) {
  case simdjson2::ondemand::json_type::array: {
    uint32_t counter = 0;
    write_byte(0xdd);
    uint8_t *location = skip_uint32();
    for (auto child : element.get_array()) {
      counter++;
      simdjson2::ondemand::value v = child.value();
      recursive_processor_ref(v);
    }
    write_uint32_at(counter, location);
  } break;
  case simdjson2::ondemand::json_type::object: {
    uint32_t counter = 0;
    write_byte(0xdf);
    uint8_t *location = skip_uint32();
    for (auto field : element.get_object()) {
      counter++;
      write_raw_string(field.key());
      simdjson2::ondemand::value v = field.value();
      recursive_processor_ref(v);
    }
    write_uint32_at(counter, location);
  } break;
  case simdjson2::ondemand::json_type::number:
    write_double(element.get_double());
    break;
  case simdjson2::ondemand::json_type::string:
    write_raw_string(element.get_raw_json_string());
    break;
  case simdjson2::ondemand::json_type::boolean:
    write_byte(0xc2 + element.get_bool());
    break;
  case simdjson2::ondemand::json_type::null:
    // We check that the value is indeed null
    // otherwise: an error is thrown.
    if(element.is_null()) {
      write_byte(0xc0);
    }
    break;
  default:
    SIMDJSON2_UNREACHABLE();
  }
}

struct simdjson2_ondemand {
  using StringType = std::string_view;

  simdjson22msgpack parser{};

  bool run(simdjson2::padded_string &json, char *buffer,
           std::string_view &result) {
    result = parser.to_msgpack(json, reinterpret_cast<uint8_t *>(buffer));
    return true;
  }
};

BENCHMARK_TEMPLATE(json2msgpack, simdjson2_ondemand)->UseManualTime();

} // namespace json2msgpack

#endif // SIMDJSON2_EXCEPTIONS
