#include "simdjson2.h"
#include <iostream>
#include <cstring>

int main(int argc, const char *argv[]) {
  std::cout << "simdjson2 v" << SIMDJSON2_VERSION << " is running the " << simdjson2::get_active_implementation()->name() << " implementation." << std::endl;
  const char *expected_implementation = nullptr;
  if (argc > 1) {
    expected_implementation = argv[1];
  } else {
    expected_implementation = getenv("SIMDJSON2_FORCE_IMPLEMENTATION");
    if (!expected_implementation) {
      std::cout << "No expected implementation argument and SIMDJSON2_FORCE_IMPLEMENTATION is not set, success by default!" << std::endl;
      return EXIT_SUCCESS;
    }
    std::cout << "No expected implementation argument, but SIMDJSON2_FORCE_IMPLEMENTATION is set to " << expected_implementation << ", so we'll check for that." << std::endl;
  }
  if (strcmp(expected_implementation, simdjson2::get_active_implementation()->name().c_str())) {
    std::cerr << "Wrong implementation! Expected " << expected_implementation << "." << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}