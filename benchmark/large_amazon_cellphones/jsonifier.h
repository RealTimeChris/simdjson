#pragma once
#if SIMDJSON_COMPETITION_JSONIFIER
#include "large_amazon_cellphones.h"
namespace large_amazon_cellphones {
template <bool threaded> struct jsonifier_off_duty {
  using StringType = std::string;
  using row_tuple =
      std::tuple<std::string, std::string, std::string, std::string,
                 std::string, double, std::string, uint64_t, std::string>;
  jsonifier::jsonifier_core<> parser{};
  bool run(simdjson::padded_string &json, std::map<StringType, brand> &result) {
    std::string_view full{json.data(), json.size()};
    size_t header_end = full.find('\n');
    if (header_end == std::string_view::npos)
      return false;
    ++header_end;
    std::string body{full.substr(header_end)};
    body.append(64, '\0');
    body.resize(body.size() - 64);
    std::vector<row_tuple> rows;
    parser.parseManyJson<std::vector<row_tuple> &,
                         jsonifier::parse_options{.partialRead = true,
                                                  .minified = true}>(rows,
                                                                     body);
    std::cerr << "[large_amazon] rows parsed: " << rows.size() << std::endl;
    for (auto &row : rows) {
      const auto &brand_name = std::get<1>(row);
      double rating = std::get<5>(row);
      uint64_t reviews = std::get<7>(row);
      auto x = result.find(brand_name);
      if (x == result.end()) {
        result.emplace(brand_name, brand{rating * reviews, reviews});
      } else {
        x->second.cumulative_rating += rating * reviews;
        x->second.reviews_count += reviews;
      }
    }
    return !rows.empty();
  }
};
BENCHMARK_TEMPLATE(large_amazon_cellphones, jsonifier_off_duty<UNTHREADED>)
    ->UseManualTime();
} // namespace large_amazon_cellphones
#endif // SIMDJSON_EXCEPTIONS
