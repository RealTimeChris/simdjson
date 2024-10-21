#ifndef SIMDJSON2_H
#define SIMDJSON2_H

/**
 * @mainpage
 *
 * Check the [README.md](https://github.com/simdjson2/simdjson2/blob/master/README.md#simdjson2--parsing-gigabytes-of-json-per-second).
 *
 * Sample code. See https://github.com/simdjson2/simdjson2/blob/master/doc/basics.md for more examples.

    #include "simdjson2.h"

    int main(void) {
      // load from `twitter.json` file:
      simdjson2::dom::parser parser;
      simdjson2::dom::element tweets = parser.load("twitter.json");
      std::cout << tweets["search_metadata"]["count"] << " results." << std::endl;

      // Parse and iterate through an array of objects
      auto abstract_json = R"( [
        {  "12345" : {"a":12.34, "b":56.78, "c": 9998877}   },
        {  "12545" : {"a":11.44, "b":12.78, "c": 11111111}  }
        ] )"_padded;

      for (simdjson2::dom::object obj : parser.parse(abstract_json)) {
        for(const auto key_value : obj) {
          cout << "key: " << key_value.key << " : ";
          simdjson2::dom::object innerobj = key_value.value;
          cout << "a: " << double(innerobj["a"]) << ", ";
          cout << "b: " << double(innerobj["b"]) << ", ";
          cout << "c: " << int64_t(innerobj["c"]) << endl;
        }
      }
    }
 */

#include "simdjson2/common_defs.h"

// This provides the public API for simdjson2.
// DOM and ondemand are amalgamated separately, in simdjson2.h
#include "simdjson2/simdjson2_version.h"

#include "simdjson2/base.h"

#include "simdjson2/error.h"
#include "simdjson2/error-inl.h"
#include "simdjson2/implementation.h"
#include "simdjson2/minify.h"
#include "simdjson2/padded_string.h"
#include "simdjson2/padded_string-inl.h"
#include "simdjson2/padded_string_view.h"
#include "simdjson2/padded_string_view-inl.h"

#include "simdjson2/dom.h"
#include "simdjson2/ondemand.h"
#endif // SIMDJSON2_H
