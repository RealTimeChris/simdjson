
#include <iostream>
#include "simdjson2.h"

using namespace simdjson2;

int main() {
    auto json = "1"_padded;
    ondemand::parser parser;
    ondemand::document doc;
    auto error = parser.iterate(json).get(doc);
    if(error) { return EXIT_FAILURE; }
    simdjson2_result<ondemand::value> query = doc.at_pointer("/");
#if COMPILATION_TEST_USE_FAILING_CODE
    if(query.second == simdjson2::SUCCESS) {
        std::cout << "success" << std::endl;
        std::cout << query.first << std::endl;
    }
#else
    ondemand::value val;
    error = query.get(val);
    if(error == simdjson2::SUCCESS) {
        std::cout << "success" << std::endl;
        std::cout << val << std::endl;
    }
#endif
    return EXIT_SUCCESS;
}