#pragma once

#if SIMDJSON2_EXCEPTIONS

#include "large_amazon_cellphones.h"
#include <algorithm>

namespace large_amazon_cellphones {

using namespace simdjson2;

template<bool threaded>
struct simdjson2_dom {
  using StringType = std::string;

  dom::parser parser{};

  bool run(simdjson2::padded_string &json, std::map<StringType, brand> &result) {
#ifdef SIMDJSON2_THREADS_ENABLED
    parser.threaded = threaded;
#endif
    auto stream = parser.parse_many(json);
    auto i = stream.begin();
    ++i;  // Skip first line
    for (;i != stream.end(); ++i) {
      auto doc = *i;
      StringType copy(std::string_view(doc.at(1)));
      auto x = result.find(copy);
      if (x == result.end()) {  // If key not found, add new key
        result.emplace(copy, large_amazon_cellphones::brand{
          double(doc.at(5)) * uint64_t(doc.at(7)),
          uint64_t(doc.at(7))
        });
      } else {  // Otherwise, update key data
        x->second.cumulative_rating += double(doc.at(5)) * uint64_t(doc.at(7));
        x->second.reviews_count += uint64_t(doc.at(7));
      }
    }

    return true;
  }

};

BENCHMARK_TEMPLATE(large_amazon_cellphones, simdjson2_dom<UNTHREADED>)->UseManualTime();
#ifdef SIMDJSON2_THREADS_ENABLED
BENCHMARK_TEMPLATE(large_amazon_cellphones, simdjson2_dom<THREADED>)->UseManualTime();
#endif

} // namespace large_amazon_cellphones

#endif // SIMDJSON2_EXCEPTIONS