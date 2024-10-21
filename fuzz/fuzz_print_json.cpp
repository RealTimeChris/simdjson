#include "simdjson2.h"
#include <cstddef>
#include <cstdint>
#include <string>

#include "NullBuffer.h"


extern "C" int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
  simdjson2::dom::parser parser;
  simdjson2::dom::element elem;
  auto error = parser.parse(Data, Size).get(elem);
  if (!error) {
    NulOStream os;
    os<<elem;
  }
  return 0;
}
