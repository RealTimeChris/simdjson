#ifdef SIMDJSON2_CONDITIONAL_INCLUDE
#error simdjson2/generic/ondemand/dependencies.h must be included before defining SIMDJSON2_CONDITIONAL_INCLUDE!
#endif

#ifndef SIMDJSON2_GENERIC_ONDEMAND_DEPENDENCIES_H
#define SIMDJSON2_GENERIC_ONDEMAND_DEPENDENCIES_H

// Internal headers needed for ondemand generics.
// All includes not under simdjson2/generic/ondemand must be here!
// Otherwise, amalgamation will fail.
#include "simdjson2/dom/base.h" // for MINIMAL_DOCUMENT_CAPACITY
#include "simdjson2/implementation.h"
#include "simdjson2/padded_string.h"
#include "simdjson2/padded_string_view.h"
#include "simdjson2/internal/dom_parser_implementation.h"
#include "simdjson2/jsonpathutil.h"

#endif // SIMDJSON2_GENERIC_ONDEMAND_DEPENDENCIES_H