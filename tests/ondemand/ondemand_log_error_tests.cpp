#define SIMDJSON2_VERBOSE_LOGGING 1
#include "simdjson2.h"
#include "test_ondemand.h"

#include <iostream>
#include <string>
#include <stdlib.h>

using namespace simdjson2;

namespace log_error_tests {
#if SIMDJSON2_EXCEPTIONS

using namespace std;

bool tape_error()
{
  TEST_START();
  auto json = R"( {"a", "hello"} )"_padded;
  ondemand::parser parser;
  try {
    ondemand::document doc = parser.iterate(json);
    std::cout << doc["a"] << std::endl;
    TEST_FAIL("Should have thrown an exception!")
  } catch (simdjson2_error& e) {
    ASSERT_ERROR(e.error(), TAPE_ERROR);
  }
  TEST_SUCCEED();
}

bool no_such_field()
{
  TEST_START();
  auto json = R"( {"a": "hello"} )"_padded;
  ondemand::parser parser;
  try {
    ondemand::document doc = parser.iterate(json);
    std::cout << doc["missing_key"] << std::endl;
    TEST_FAIL("Should have thrown an exception!")
  } catch (simdjson2_error& e) {
    ASSERT_ERROR(e.error(), NO_SUCH_FIELD);
  }
  TEST_SUCCEED();
}
#endif // SIMDJSON2_EXCEPTIONS

bool run()
{
  SIMDJSON2_PUSH_DISABLE_WARNINGS
  SIMDJSON2_DISABLE_DEPRECATED_WARNING // Disable CRT_SECURE warning on MSVC: manually verified this is safe
  std::string str = "SIMDJSON2_LOG_LEVEL=ERROR";
  putenv(str.data());
  bool rc =
#if SIMDJSON2_EXCEPTIONS
            tape_error() &&
            no_such_field() &&
#endif // #if SIMDJSON2_EXCEPTIONS
            true;
  SIMDJSON2_POP_DISABLE_WARNINGS
  return rc;
}

}

int main(int argc, char *argv[]) {
  return test_main(argc, argv, log_error_tests::run);
}
