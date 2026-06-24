#include "simdjson.h"

SIMDJSON_PUSH_DISABLE_ALL_WARNINGS

#ifdef SIMDJSON_COMPETITION_JSONIFIER
#include <jsonifier>
#endif

// This has to be last, for reasons I don't yet understand
#include <benchmark/benchmark.h>

#include "twitter_full/simdjson_ondemand.h"
#include "twitter_full/jsonifier.h"
#include "distinct_user_id/simdjson_ondemand.h"
#include "distinct_user_id/jsonifier.h"
#include "kostya/simdjson_ondemand.h"
#include "kostya/jsonifier.h"
#include "large_random/simdjson_ondemand.h"
#include "large_random/jsonifier.h"

BENCHMARK_MAIN();
