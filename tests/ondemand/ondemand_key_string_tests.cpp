#include "simdjson2.h"
#include "test_ondemand.h"

using namespace simdjson2;

namespace key_string_tests {
#if SIMDJSON2_EXCEPTIONS
  bool parser_key_value() {
    TEST_START();
    ondemand::parser parser;
    const padded_string json = R"({ "1": "1", "2": "2", "3": "3", "abc": "abc", "\u0075": "\u0075" })"_padded;
    auto doc = parser.iterate(json);
    for(auto field : doc.get_object())  {
      std::string_view keyv = field.unescaped_key();
      std::string_view valuev = field.value();
      if(keyv != valuev) { return false; }
    }
    return true;
  }

  bool parser_escaped_key() {
    TEST_START();
    ondemand::parser parser;
    const padded_string json = "{ \"1\": \"1\", \"2\"   : \"2\", \"3\" \t : \"3\", \"abc\"\n\t\n: \"abc\", \"\\u0075\": \"\\\\u0075\" }"_padded;
    auto doc = parser.iterate(json);
    for(auto field : doc.get_object())  {
      std::string_view keyv = field.escaped_key();
      std::string_view valuev = field.value();
      if(keyv != valuev) { return false; }
    }
    return true;
  }

#endif // SIMDJSON2_EXCEPTIONS
  bool run() {
    return
#if SIMDJSON2_EXCEPTIONS
      parser_key_value() &&
      parser_escaped_key() &&
#endif // SIMDJSON2_EXCEPTIONS
      true;
  }

} // namespace key_string_tests

int main(int argc, char *argv[]) {
  return test_main(argc, argv, key_string_tests::run);
}
