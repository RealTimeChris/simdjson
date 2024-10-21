#include "simdjson2.h"
#include <iostream>

using namespace simdjson2;

int main(void) {
   ondemand::parser parser;
   auto cars_json = R"( { "test":[ { "val1":1, "val2":2 }, { "val1":1, "val2":2 } ] }   )"_padded;
   auto doc = parser.iterate(cars_json);
   auto testArray = doc.find_field("test");
#if COMPILATION_TEST_USE_FAILING_CODE
   size_t count = testArray.get_array().count_elements();
   std::cout << "Number of elements: " <<  count << std::endl;
   for(simdjson2_unused ondemand::object elem : testArray) {}
#else
   ondemand::array a = testArray.get_array();
   size_t count = a.count_elements();
   std::cout << "Number of elements: " <<  count << std::endl;
   for(simdjson2_unused ondemand::object elem : a) {}
#endif
}