#ifndef SIMDJSON2_SRC_GENERIC_STAGE2_BASE_H

#ifndef SIMDJSON2_CONDITIONAL_INCLUDE
#define SIMDJSON2_SRC_GENERIC_STAGE2_BASE_H
#include <generic/base.h>
#endif // SIMDJSON2_CONDITIONAL_INCLUDE

namespace simdjson2 {
namespace SIMDJSON2_IMPLEMENTATION {
namespace {
namespace stage2 {

class json_iterator;
class structural_iterator;
struct tape_builder;
struct tape_writer;

} // namespace stage2
} // unnamed namespace
} // namespace SIMDJSON2_IMPLEMENTATION
} // namespace simdjson2

#endif // SIMDJSON2_SRC_GENERIC_STAGE2_BASE_H