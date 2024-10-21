#ifdef SIMDJSON2_CONDITIONAL_INCLUDE
#error simdjson2/generic/dependencies.h must be included before defining SIMDJSON2_CONDITIONAL_INCLUDE!
#endif

#ifndef SIMDJSON2_GENERIC_DEPENDENCIES_H
#define SIMDJSON2_GENERIC_DEPENDENCIES_H

// Internal headers needed for generics.
// All includes referencing simdjson2 headers *not* under simdjson2/generic must be here!
// Otherwise, amalgamation will fail.
#include "simdjson2/base.h"
#include "simdjson2/implementation.h"
#include "simdjson2/implementation_detection.h"
#include "simdjson2/internal/instruction_set.h"
#include "simdjson2/internal/dom_parser_implementation.h"
#include "simdjson2/internal/jsoncharutils_tables.h"
#include "simdjson2/internal/numberparsing_tables.h"
#include "simdjson2/internal/simdprune_tables.h"
#endif // SIMDJSON2_GENERIC_DEPENDENCIES_H
