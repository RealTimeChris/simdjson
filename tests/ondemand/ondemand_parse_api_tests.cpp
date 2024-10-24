#include "simdjson2.h"
#include "test_ondemand.h"

using namespace simdjson2;

namespace parse_api_tests {
  using namespace std;

  const padded_string BASIC_JSON = "[1,2,3]"_padded;
  const padded_string BASIC_NDJSON = "[1,2,3]\n[4,5,6]"_padded;
  const padded_string EMPTY_NDJSON = ""_padded;


  bool parser_iterate_empty() {
    TEST_START();
    FILE *p;
    // Of course, we could just call iterate on the empty string, but
    // we want to test the whole process.
    const char *const tmpfilename = "emptyondemand.txt";
    if((p = fopen(tmpfilename, "w")) != nullptr) {
      fclose(p);
      auto json = padded_string::load(tmpfilename);
      ondemand::document doc;
      ondemand::parser parser;
      auto error = parser.iterate(json).get(doc);
      remove(tmpfilename);
      if(error != simdjson2::EMPTY) {
        std::cerr << "Was expecting empty but got " << error << std::endl;
        return false;
      }
    } else {
      std::cout << "Warning: I could not create temporary file " << tmpfilename << std::endl;
      std::cout << "We omit testing the empty file case." << std::endl;
    }
    TEST_SUCCEED();
  }

  bool parser_iterate() {
    TEST_START();
    ondemand::parser parser;
    auto doc = parser.iterate(BASIC_JSON);
    ASSERT_SUCCESS( doc.get_array() );
    return true;
  }

  bool parser_iterate_padded() {
    TEST_START();
    ondemand::parser parser;
    const char json_str[] = "12\0                                                              ";// 64 bytes of padding
    ASSERT_EQUAL(sizeof(json_str), 66);
    ASSERT_EQUAL(strlen(json_str), 2);

    {
      cout << "- char*" << endl;
      auto doc = parser.iterate(json_str, strlen(json_str), sizeof(json_str));
      ASSERT_SUCCESS( doc.get_double() );
    }

    {
      cout << "- uint8_t*" << endl;
      const uint8_t* json = reinterpret_cast<const uint8_t*>(json_str);
      auto doc = parser.iterate(json, strlen(json_str), sizeof(json_str));
      ASSERT_SUCCESS( doc.get_double() );
    }

    {
      cout << "- string_view" << endl;
      std::string_view json(json_str);
      auto doc = parser.iterate(json, sizeof(json_str));
      ASSERT_SUCCESS( doc.get_double() );
    }

    {
      cout << "- string" << endl;
      std::string json = "12";
      json.reserve(json.length() + SIMDJSON2_PADDING);
      auto doc = parser.iterate(json);
      ASSERT_SUCCESS( doc.get_double() );
    }

    TEST_SUCCEED();
  }

  bool parser_iterate_padded_string_view() {
    TEST_START();
    ondemand::parser parser;
    const char json_str[] = "12\0                                                              "; // 64 bytes of padding
    ASSERT_EQUAL(sizeof(json_str), 66);
    ASSERT_EQUAL(strlen(json_str), 2);

    {
      cout << "- padded_string_view(string_view)" << endl;
      padded_string_view json(std::string_view(json_str), sizeof(json_str));
      auto doc = parser.iterate(json);
      ASSERT_SUCCESS( doc.get_double() );
    }

    {
      cout << "- padded_string_view(char*)" << endl;
      auto doc = parser.iterate(padded_string_view(json_str, strlen(json_str), sizeof(json_str)));
      ASSERT_SUCCESS( doc.get_double() );
    }

    {
      cout << "- padded_string_view(string)" << endl;
      std::string json = "12";
      json.reserve(json.length() + SIMDJSON2_PADDING);
      auto doc = parser.iterate(padded_string_view(json));
      ASSERT_SUCCESS( doc.get_double() );
    }

    {
      cout << "- padded_string_view(string_view(char*))" << endl;
      padded_string_view json(json_str, sizeof(json_str));
      auto doc = parser.iterate(json);
      ASSERT_SUCCESS( doc.get_double() );
    }

    TEST_SUCCEED();
  }

  bool parser_iterate_insufficient_padding() {
    TEST_START();
    ondemand::parser parser;
    constexpr char json_str[] = "12\0                                                             "; // 63 bytes of padding
    ASSERT_EQUAL(sizeof(json_str), 65);
    ASSERT_EQUAL(strlen(json_str), 2);
    ASSERT_EQUAL(padded_string_view(json_str, strlen(json_str), sizeof(json_str)).padding(), 63);
    ASSERT_EQUAL(SIMDJSON2_PADDING, 64);

    {
      cout << "- char*, 63 padding" << endl;
      ASSERT_ERROR( parser.iterate(json_str, strlen(json_str), sizeof(json_str)), INSUFFICIENT_PADDING );
      cout << "- char*, 0 padding" << endl;
      ASSERT_ERROR( parser.iterate(json_str, strlen(json_str), strlen(json_str)), INSUFFICIENT_PADDING );
    }

    {
      std::string_view json(json_str);
      cout << "- string_view, 63 padding" << endl;
      ASSERT_ERROR( parser.iterate(json, sizeof(json_str)), INSUFFICIENT_PADDING );
      cout << "- string_view, 0 padding" << endl;
      ASSERT_ERROR( parser.iterate(json, strlen(json_str)), INSUFFICIENT_PADDING );
    }

    {
      std::string json = "12345642314123421321321321321321312321321321321312";
      json.shrink_to_fit();
      cout << "- string, 0 padding" << endl;
      ASSERT_SUCCESS( parser.iterate(json) );
    }

    {
      std::string json = "12345642314123421321321321321321312321321321321312";
      json.shrink_to_fit();
      cout << "- string, 0 padding" << endl;
      ASSERT_ERROR( parser.iterate((const std::string&)json), INSUFFICIENT_PADDING );
      // It's actually kind of hard to allocate "just enough" capacity, since the string tends
      // to grow more than you tell it to.
    }
    TEST_SUCCEED();
  }

#if SIMDJSON2_EXCEPTIONS
  bool parser_iterate_exception() {
    TEST_START();
    ondemand::parser parser;
    auto doc = parser.iterate(BASIC_JSON);
    simdjson2_unused ondemand::array array = doc;
    TEST_SUCCEED();
  }

  bool parser_document_reuse() {
    TEST_START();
    ondemand::document doc;
    // A document spans about 40 bytes. Nevertheless, some users
    // would rather reuse them.
    std::cout << sizeof(doc) << std::endl;
    auto json = R"({"key": "value"})"_padded;
    auto jsonbad = R"({"key": "value")"_padded; // deliberaty broken
    auto jsonunclosedstring = "{\"coordinates:[{\"x\":1.1,\"y\":2.2,\"z\":3.3}]}"_padded;
    std::string_view output;

    ondemand::parser parser;
    std::cout << "correct document (1)" << std::endl;

    ASSERT_SUCCESS( parser.iterate(json).get(doc) );

    ASSERT_SUCCESS(simdjson2::to_json_string(doc).get(output));
    std::cout << output << std::endl;

    std::cout << "correct document (2)" << std::endl;

    ASSERT_SUCCESS( parser.iterate(json).get(doc) );
    for(ondemand::field field : doc.get_object() ) {
      std::cout << "field: " << field.key() << std::endl;
    }
    std::cout << "unclosed string document " << std::endl;
    simdjson2::error_code error;
    if((error = parser.iterate(jsonunclosedstring).get(doc)) == SUCCESS) {
      // fallback kernel:
      ASSERT_EQUAL( doc.get_object().find_field("coordinates").error(), TAPE_ERROR );
    } else {
      // regular kernels:
      ASSERT_EQUAL( error, UNCLOSED_STRING );
    }

    std::cout << "truncated document " << std::endl;
    ASSERT_SUCCESS( parser.iterate(jsonbad).get(doc) );
    ASSERT_EQUAL( simdjson2::to_json_string(doc).get(output), TAPE_ERROR );

    std::cout << "correct document with new doc" << std::endl;
    ondemand::document doc2;
    ASSERT_SUCCESS( parser.iterate(json).get(doc2) );
    for(ondemand::field field : doc2.get_object() ) {
      std::cout << "field: " << field.key() << std::endl;
    }
    std::cout << "correct document (3): " << doc.to_debug_string() << std::endl;

    std::cout << "correct document (3)" << std::endl;
    ASSERT_SUCCESS( parser.iterate(json).get(doc) );
    std::cout << doc.to_debug_string() << std::endl;
    for(ondemand::field field : doc.get_object() ) {
      std::cout << "field: " << field.key() << std::endl;
    }

    std::cout << "unclosed string document " << std::endl;
    ASSERT_SUCCESS( parser.iterate(jsonbad).get(doc) );
    ASSERT_EQUAL( simdjson2::to_json_string(doc).get(output), TAPE_ERROR );

    // next two lines are terrible code.
    doc.~document();
    doc = ondemand::document();
    //

    std::cout << "correct document (4)" << std::endl;

    ASSERT_SUCCESS( parser.iterate(json).get(doc) );
    ASSERT_SUCCESS( simdjson2::to_json_string(doc).get(output) );
    std::cout << output << std::endl;

    std::cout << "unclosed string document " << std::endl;

    if((error = parser.iterate(jsonunclosedstring).get(doc)) == SUCCESS) {
      // fallback kernel:
      ASSERT_EQUAL( doc.get_object().find_field("coordinates").error(), TAPE_ERROR );
    } else {
      // regular kernels:
      ASSERT_EQUAL( error, UNCLOSED_STRING );
    }


    // next two lines are terrible code.
    doc.~document();
    doc = ondemand::document();
    //
    std::cout << "correct document (5)" << std::endl;

    ASSERT_SUCCESS( parser.iterate(json).get(doc) );
    ASSERT_SUCCESS( simdjson2::to_json_string(doc).get(output) );
    std::cout << output << std::endl;

    TEST_SUCCEED();
  }
#endif // SIMDJSON2_EXCEPTIONS

  bool run() {
    return parser_iterate_empty() &&
           parser_iterate() &&
           parser_iterate_padded() &&
           parser_iterate_padded_string_view() &&
           parser_iterate_insufficient_padding() &&
#if SIMDJSON2_EXCEPTIONS
           parser_document_reuse() &&
           parser_iterate_exception() &&
#endif // SIMDJSON2_EXCEPTIONS
           true;
  }
}


int main(int argc, char *argv[]) {
  return test_main(argc, argv, parse_api_tests::run);
}
