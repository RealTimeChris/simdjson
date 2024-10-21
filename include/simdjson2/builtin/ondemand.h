#ifndef SIMDJSON2_BUILTIN_ONDEMAND_H
#define SIMDJSON2_BUILTIN_ONDEMAND_H

#include "simdjson2/builtin.h"
#include "simdjson2/builtin/base.h"

#include "simdjson2/generic/ondemand/dependencies.h"

#define SIMDJSON2_CONDITIONAL_INCLUDE

#if SIMDJSON2_BUILTIN_IMPLEMENTATION_IS(arm64)
#include "simdjson2/arm64/ondemand.h"
#elif SIMDJSON2_BUILTIN_IMPLEMENTATION_IS(fallback)
#include "simdjson2/fallback/ondemand.h"
#elif SIMDJSON2_BUILTIN_IMPLEMENTATION_IS(haswell)
#include "simdjson2/haswell/ondemand.h"
#elif SIMDJSON2_BUILTIN_IMPLEMENTATION_IS(icelake)
#include "simdjson2/icelake/ondemand.h"
#elif SIMDJSON2_BUILTIN_IMPLEMENTATION_IS(ppc64)
#include "simdjson2/ppc64/ondemand.h"
#elif SIMDJSON2_BUILTIN_IMPLEMENTATION_IS(westmere)
#include "simdjson2/westmere/ondemand.h"
#elif SIMDJSON2_BUILTIN_IMPLEMENTATION_IS(lsx)
#include "simdjson2/lsx/ondemand.h"
#elif SIMDJSON2_BUILTIN_IMPLEMENTATION_IS(lasx)
#include "simdjson2/lasx/ondemand.h"
#else
#error Unknown SIMDJSON2_BUILTIN_IMPLEMENTATION
#endif

#undef SIMDJSON2_CONDITIONAL_INCLUDE

namespace simdjson2 {
  /**
   * @copydoc simdjson2::SIMDJSON2_BUILTIN_IMPLEMENTATION::ondemand
   */
  namespace ondemand = SIMDJSON2_BUILTIN_IMPLEMENTATION::ondemand;
} // namespace simdjson2

#endif // SIMDJSON2_BUILTIN_ONDEMAND_H