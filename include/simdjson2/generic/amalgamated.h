#if defined(SIMDJSON2_CONDITIONAL_INCLUDE) && !defined(SIMDJSON2_GENERIC_DEPENDENCIES_H)
#error simdjson2/generic/dependencies.h must be included before simdjson2/generic/amalgamated.h!
#endif

#include "simdjson2/generic/base.h"
#include "simdjson2/generic/jsoncharutils.h"
#include "simdjson2/generic/atomparsing.h"
#include "simdjson2/generic/dom_parser_implementation.h"
#include "simdjson2/generic/implementation_simdjson_result_base.h"
#include "simdjson2/generic/numberparsing.h"

#include "simdjson2/generic/implementation_simdjson_result_base-inl.h"
