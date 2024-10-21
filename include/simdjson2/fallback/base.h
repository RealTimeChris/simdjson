#ifndef SIMDJSON2_FALLBACK_BASE_H
#define SIMDJSON2_FALLBACK_BASE_H

#ifndef SIMDJSON2_CONDITIONAL_INCLUDE
#include "simdjson2/base.h"
#endif // SIMDJSON2_CONDITIONAL_INCLUDE

namespace simdjson2 {
/**
 * Fallback implementation (runs on any machine).
 */
namespace fallback {

class implementation;

} // namespace fallback
} // namespace simdjson2

#endif // SIMDJSON2_FALLBACK_BASE_H
