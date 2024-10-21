#pragma once

#if SIMDJSON2_EXCEPTIONS

#include "largerandom.h"

namespace largerandom {

using namespace simdjson2;

class Iter {
public:
  simdjson2_inline bool Run(const padded_string &json);

  simdjson2_inline const std::vector<my_point> &Result() { return container; }
  simdjson2_inline size_t ItemCount() { return container.size(); }

private:
  ondemand::parser parser{};
  std::vector<my_point> container{};

  simdjson2_inline double first_double(ondemand::json_iterator &iter) {
    if (iter.start_object().error() || iter.field_key().error() || iter.field_value()) { throw "Invalid field"; }
    return iter.consume_double();
  }

  simdjson2_inline double next_double(ondemand::json_iterator &iter) {
    if (!iter.has_next_field() || iter.field_key().error() || iter.field_value()) { throw "Invalid field"; }
    return iter.consume_double();
  }

};

simdjson2_inline bool Iter::Run(const padded_string &json) {
  container.clear();

  auto iter = parser.iterate_raw(json).value();
  if (iter.start_array()) {
    do {
      container.emplace_back(my_point{first_double(iter), next_double(iter), next_double(iter)});
      if (iter.has_next_field()) { throw "Too many fields"; }
    } while (iter.has_next_element());
  }

  return true;
}

BENCHMARK_TEMPLATE(LargeRandom, Iter);

} // namespace largerandom

#endif // SIMDJSON2_EXCEPTIONS
