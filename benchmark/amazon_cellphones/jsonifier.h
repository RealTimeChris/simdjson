#pragma once

#ifdef SIMDJSON_COMPETITION_JSONIFIER

#include "amazon_cellphones.h"

using value_type = jsonifier_internal::tuple<jsonifier::skip, std::string, jsonifier::skip, jsonifier::skip, jsonifier::skip, double, jsonifier::skip, uint64_t, jsonifier::skip>;

namespace amazon_cellphones {

template <bool threaded> struct Jsonifier {
  using StringType = std::string;

  jsonifier::jsonifier_core<> parser{};

  bool run(simdjson::padded_string &json, std::map<std::string, brand> &result) {

    auto newJson = std::string_view{json.data(), json.size()};
    newJson.remove_prefix(newJson.find('\n') + 1);

    std::vector<value_type> results;
    results.reserve(1000);

    auto resultNew =
        parser.parseManyJson<std::vector<value_type> &,
                             jsonifier::parse_options{.knownOrder = false}>(
            results, newJson);

    for (auto &docNew : results) {
      auto copy = jsonifier_internal::get<1>(docNew);
      auto rating = jsonifier_internal::get<5>(docNew);
      auto reviews = jsonifier_internal::get<7>(docNew);

      auto [iter, inserted] = result.try_emplace(
          std::move(copy), amazon_cellphones::brand{rating * reviews, reviews});

      if (!inserted) {
        iter->second.cumulative_rating += rating * reviews;
        iter->second.reviews_count += reviews;
      }
    }

    return resultNew;
  }
};

BENCHMARK_TEMPLATE(amazon_cellphones, Jsonifier<UNTHREADED>)
    ->UseManualTime();
#ifdef SIMDJSON_THREADS_ENABLED
BENCHMARK_TEMPLATE(amazon_cellphones, Jsonifier<THREADED>)->UseManualTime();
#endif

} // namespace amazon_cellphones

#endif // SIMDJSON_EXCEPTIONS
