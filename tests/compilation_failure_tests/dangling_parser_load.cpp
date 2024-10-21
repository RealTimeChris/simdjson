
// this tests https://github.com/simdjson2/simdjson2/issues/696

#include <iostream>
#include "simdjson2.h"

int main() {
#if COMPILATION_TEST_USE_FAILING_CODE
    simdjson2::dom::element tree = simdjson2::dom::parser().load("tree-pretty.json");
#else
    simdjson2::dom::parser parser;
    simdjson2::dom::element tree = parser.load("tree-pretty.json");
#endif
    std::cout << tree["type"] << std::endl;
}
