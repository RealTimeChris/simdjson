#include <iostream>
#include "simdjson2.h"

int main() {
  simdjson2::error_code error;
  simdjson2::padded_string numberstring = "1.2"_padded; // our JSON input ("1.2")
  simdjson2::dom::parser parser;
  double value; // variable where we store the value to be parsed
  error = parser.parse(numberstring).get(value);
  if (error) { std::cerr << error << std::endl; return EXIT_FAILURE; }
  std::cout << "I parsed " << value << " from " << numberstring.data() << std::endl;
}
