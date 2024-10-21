#include <iostream>
#include "simdjson2.h"

int main(void) {
  simdjson2::dom::parser parser;
  simdjson2::dom::element tweets = parser.load("twitter.json");
  std::cout << tweets["search_metadata"]["count"] << " results." << std::endl;
}
