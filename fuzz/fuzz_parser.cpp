#include "simdjson2.h"
#include <cstddef>
#include <cstdint>
#include <string>
extern "C" int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
  simdjson2::dom::parser parser;
  simdjson2_unused simdjson2::dom::element elem;
  simdjson2_unused auto error = parser.parse(Data, Size).get(elem);
  return 0;
}
