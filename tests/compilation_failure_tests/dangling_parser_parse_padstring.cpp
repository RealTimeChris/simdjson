
// this tests https://github.com/simdjson2/simdjson2/issues/696

#include <iostream>
#include "simdjson2.h"

int main() {
    simdjson2::padded_string buf;
#if COMPILATION_TEST_USE_FAILING_CODE
    simdjson2::dom::element tree = simdjson2::dom::parser().parse(buf);
#else
    simdjson2::dom::parser parser;
    simdjson2::dom::element tree = parser.parse(buf);
#endif
    std::cout << tree["type"] << std::endl;
}
