#pragma once

#ifdef SIMDJSON_COMPETITION_JSONIFIER

#include "large_amazon_cellphones.h"
#include <algorithm>
#include <jsonifier/Index.hpp>

namespace large_amazon_cellphones {

using value_type =
    jsonifier_internal::tuple<jsonifier::skip, std::string, jsonifier::skip,
                              jsonifier::skip, jsonifier::skip, double,
                              jsonifier::skip, uint64_t, jsonifier::skip>;

template <bool threaded> struct Jsonifier {
  using StringType = std::string;

  jsonifier::jsonifier_core<> parser{};

  bool run(simdjson::padded_string &json,
           std::map<std::string, brand> &result) {

     auto newJson = std::string_view{json.data(), json.size()};
    newJson.remove_prefix(newJson.find('\n') + 1);

    std::vector<value_type> results;
    results.reserve(1000);

    auto resultNew =
        parser.template parseManyJson<std::vector<value_type>&>(
            results, json);

    for (auto &docNew : results) {
      auto copy = jsonifier_internal::get<1>(docNew);
      auto rating = jsonifier_internal::get<5>(docNew);
      auto reviews = jsonifier_internal::get<7>(docNew);

      auto [iter, inserted] = result.try_emplace(
          std::move(copy), static_cast<double>(rating * reviews));

      auto x = result.find(copy);
      if (x == result.end()) {
        result.emplace(
            copy, large_amazon_cellphones::brand{rating * reviews, reviews});
      } else {
        x->second.cumulative_rating += rating * reviews;
        x->second.reviews_count += reviews;
      }
    }

    return resultNew;
  }
};

BENCHMARK_TEMPLATE(large_amazon_cellphones, Jsonifier<UNTHREADED>)
    ->UseManualTime();
#ifdef SIMDJSON_THREADS_ENABLED
BENCHMARK_TEMPLATE(large_amazon_cellphones, Jsonifier<THREADED>)
    ->UseManualTime();
#endif

} // namespace large_amazon_cellphones

#endif // SIMDJSON_EXCEPTIONS
