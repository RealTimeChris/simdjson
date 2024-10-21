#include "FuzzUtils.h"
#include "simdjson2.h"
#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {

  // Split data into two strings, JSON Pointer and the document string.
  // Might end up with none, either or both being empty, important for
  // covering edge cases such as
  // https://github.com/simdjson2/simdjson2/issues/1142 Inputs missing the
  // separator line will get an empty JSON Pointer but the all the input put in
  // the document string. This means test data from other fuzzers that take json
  // input works for this fuzzer as well.
  FuzzData fd(Data, Size);
  auto strings = fd.splitIntoStrings();
  while (strings.size() < 2) {
    strings.emplace_back();
  }
  assert(strings.size() >= 2);

  simdjson2::dom::parser parser;

  // parse without exceptions, for speed
  auto res = parser.parse(strings[0]);
  if (res.error())
    return 0;

  simdjson2::dom::element root;
  if (res.get(root))
    return 0;

  auto maybe_leaf = root.at_pointer(strings[1]);
  if (maybe_leaf.error())
    return 0;

  simdjson2::dom::element leaf;
  if (maybe_leaf.get(leaf))
    return 0;

  std::string_view sv;
  if (leaf.get_string().get(sv))
    return 0;
  return 0;
}
