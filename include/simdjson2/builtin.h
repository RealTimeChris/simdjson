#ifndef SIMDJSON2_BUILTIN_H
#define SIMDJSON2_BUILTIN_H

#include "simdjson2/builtin/base.h"
#include "simdjson2/builtin/implementation.h"

#include "simdjson2/generic/dependencies.h"

#define SIMDJSON2_CONDITIONAL_INCLUDE

#if SIMDJSON2_BUILTIN_IMPLEMENTATION_IS(arm64)
#include "simdjson2/arm64.h"
#elif SIMDJSON2_BUILTIN_IMPLEMENTATION_IS(fallback)
#include "simdjson2/fallback.h"
#elif SIMDJSON2_BUILTIN_IMPLEMENTATION_IS(haswell)
#include "simdjson2/haswell.h"
#elif SIMDJSON2_BUILTIN_IMPLEMENTATION_IS(icelake)
#include "simdjson2/icelake.h"
#elif SIMDJSON2_BUILTIN_IMPLEMENTATION_IS(ppc64)
#include "simdjson2/ppc64.h"
#elif SIMDJSON2_BUILTIN_IMPLEMENTATION_IS(westmere)
#include "simdjson2/westmere.h"
#elif SIMDJSON2_BUILTIN_IMPLEMENTATION_IS(lsx)
#include "simdjson2/lsx.h"
#elif SIMDJSON2_BUILTIN_IMPLEMENTATION_IS(lasx)
#include "simdjson2/lasx.h"
#else
#error Unknown SIMDJSON2_BUILTIN_IMPLEMENTATION
#endif

#undef SIMDJSON2_CONDITIONAL_INCLUDE

#endif // SIMDJSON2_BUILTIN_H
