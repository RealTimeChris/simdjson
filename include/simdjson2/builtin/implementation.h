#ifndef SIMDJSON2_BUILTIN_IMPLEMENTATION_H
#define SIMDJSON2_BUILTIN_IMPLEMENTATION_H

#include "simdjson2/builtin/base.h"

#include "simdjson2/generic/dependencies.h"

#define SIMDJSON2_CONDITIONAL_INCLUDE

#if SIMDJSON2_BUILTIN_IMPLEMENTATION_IS(arm64)
#include "simdjson2/arm64/implementation.h"
#elif SIMDJSON2_BUILTIN_IMPLEMENTATION_IS(fallback)
#include "simdjson2/fallback/implementation.h"
#elif SIMDJSON2_BUILTIN_IMPLEMENTATION_IS(haswell)
#include "simdjson2/haswell/implementation.h"
#elif SIMDJSON2_BUILTIN_IMPLEMENTATION_IS(icelake)
#include "simdjson2/icelake/implementation.h"
#elif SIMDJSON2_BUILTIN_IMPLEMENTATION_IS(ppc64)
#include "simdjson2/ppc64/implementation.h"
#elif SIMDJSON2_BUILTIN_IMPLEMENTATION_IS(westmere)
#include "simdjson2/westmere/implementation.h"
#elif SIMDJSON2_BUILTIN_IMPLEMENTATION_IS(lsx)
#include "simdjson2/lsx/implementation.h"
#elif SIMDJSON2_BUILTIN_IMPLEMENTATION_IS(lasx)
#include "simdjson2/lasx/implementation.h"
#else
#error Unknown SIMDJSON2_BUILTIN_IMPLEMENTATION
#endif

#undef SIMDJSON2_CONDITIONAL_INCLUDE

namespace simdjson2 {
  /**
   * Function which returns a pointer to an implementation matching the "builtin" implementation.
   * The builtin implementation is the best statically linked simdjson2 implementation that can be used by the compiling
   * program. If you compile with g++ -march=haswell, this will return the haswell implementation.
   * It is handy to be able to check what builtin was used: builtin_implementation()->name().
   */
  const implementation * builtin_implementation();
} // namespace simdjson2

#endif // SIMDJSON2_BUILTIN_IMPLEMENTATION_H