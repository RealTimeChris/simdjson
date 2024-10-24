#include <iostream>
#include "simdjson2.h"

int main(void) {
  simdjson2::dom::parser parser;
  simdjson2::dom::element tweets;
  auto error = parser.load("twitter.json").get(tweets);
  if(error) { std::cerr << error << std::endl; return EXIT_FAILURE; }
  uint64_t identifier;
  error = tweets["statuses"].at(0)["id"].get(identifier);
  if(error) { std::cerr << error << std::endl; return EXIT_FAILURE; }
  std::cout << identifier << std::endl;
  return EXIT_SUCCESS;
}
