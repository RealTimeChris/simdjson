#ifndef SIMDJSON2_BUILTIN_BASE_H
#define SIMDJSON2_BUILTIN_BASE_H

#include "simdjson2/base.h"
#include "simdjson2/implementation_detection.h"

namespace simdjson2 {
#if SIMDJSON2_BUILTIN_IMPLEMENTATION_IS(arm64)
  namespace arm64 {}
#elif SIMDJSON2_BUILTIN_IMPLEMENTATION_IS(fallback)
  namespace fallback {}
#elif SIMDJSON2_BUILTIN_IMPLEMENTATION_IS(haswell)
  namespace haswell {}
#elif SIMDJSON2_BUILTIN_IMPLEMENTATION_IS(icelake)
  namespace icelake {}
#elif SIMDJSON2_BUILTIN_IMPLEMENTATION_IS(ppc64)
  namespace ppc64 {}
#elif SIMDJSON2_BUILTIN_IMPLEMENTATION_IS(westmere)
  namespace westmere {}
#elif SIMDJSON2_BUILTIN_IMPLEMENTATION_IS(lsx)
  namespace lsx {}
#elif SIMDJSON2_BUILTIN_IMPLEMENTATION_IS(lasx)
  namespace lasx {}
#else
#error Unknown SIMDJSON2_BUILTIN_IMPLEMENTATION
#endif

  /**
   * Represents the best statically linked simdjson2 implementation that can be used by the compiling
   * program.
   *
   * Detects what options the program is compiled against, and picks the minimum implementation that
   * will work on any computer that can run the program. For example, if you compile with g++
   * -march=westmere, it will pick the westmere implementation. The haswell implementation will
   * still be available, and can be selected at runtime, but the builtin implementation (and any
   * code that uses it) will use westmere.
   */
  namespace builtin = SIMDJSON2_BUILTIN_IMPLEMENTATION;
} // namespace simdjson2

#endif // SIMDJSON2_BUILTIN_BASE_H
