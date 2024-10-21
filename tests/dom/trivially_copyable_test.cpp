// This file is not part of our main, regular tests.
#include "../singleheader/simdjson2.h"
#include <iostream>
#include <type_traits>

int main() {
  if (!std::is_trivially_copyable<simdjson2::dom::element>::value) {
    std::cerr << "simdjson2::dom::element must be trivially copyable"
              << std::endl;
    return EXIT_FAILURE;
  }
  if (!std::is_trivially_copyable<simdjson2::dom::array>::value) {
    std::cerr << "simdjson2::dom::array must be trivially copyable" << std::endl;
    return EXIT_FAILURE;
  }
  if (!std::is_trivially_copyable<simdjson2::dom::object>::value) {
    std::cerr << "simdjson2::dom::object must be trivially copyable"
              << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
