// This file is not part of our main, regular tests.
#include "../singleheader/simdjson2.h"
#include <iostream>

using namespace simdjson2;

int main() {
  const char *filename = SIMDJSON2_BENCHMARK_DATA_DIR "/twitter.json";
  dom::parser parser;
  dom::element doc;
  auto error = parser.load(filename).get(doc);
  if (error) {
    std::cerr << error << std::endl;
    return EXIT_FAILURE;
  }
  std::cout << doc << std::endl;
  return EXIT_SUCCESS;
}
