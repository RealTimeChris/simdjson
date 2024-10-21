#pragma once

#if SIMDJSON2_EXCEPTIONS

#include "largerandom.h"

namespace largerandom {

using namespace simdjson2;

class OnDemand {
public:
  simdjson2_inline bool Run(const padded_string &json);
  simdjson2_inline const std::vector<my_point> &Result() { return container; }
  simdjson2_inline size_t ItemCount() { return container.size(); }

private:
  ondemand::parser parser{};
  std::vector<my_point> container{};
};

simdjson2_inline bool OnDemand::Run(const padded_string &json) {
  container.clear();

  auto doc = parser.iterate(json);
  for (ondemand::object coord : doc) {
    container.emplace_back(my_point{coord.find_field("x"), coord.find_field("y"), coord.find_field("z")});
  }

  return true;
}

BENCHMARK_TEMPLATE(LargeRandom, OnDemand);

} // namespace largerandom

#endif // SIMDJSON2_EXCEPTIONS
