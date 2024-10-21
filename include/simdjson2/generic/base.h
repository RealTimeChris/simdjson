#ifndef SIMDJSON2_GENERIC_BASE_H

#ifndef SIMDJSON2_CONDITIONAL_INCLUDE
#define SIMDJSON2_GENERIC_BASE_H
#include "simdjson2/base.h"
// If we haven't got an implementation yet, we're in the editor, editing a generic file! Just
// use the most advanced one we can so the most possible stuff can be tested.
#ifndef SIMDJSON2_IMPLEMENTATION
#include "simdjson2/implementation_detection.h"
#if SIMDJSON2_IMPLEMENTATION_ICELAKE
//#include "simdjson2/icelake/begin.h"
#elif SIMDJSON2_IMPLEMENTATION_HASWELL
#include "simdjson2/haswell/begin.h"
#elif SIMDJSON2_IMPLEMENTATION_WESTMERE
// #include "simdjson2/westmere/begin.h"
#elif SIMDJSON2_IMPLEMENTATION_ARM64
 #include "simdjson2/arm64/begin.h"
#elif SIMDJSON2_IMPLEMENTATION_PPC64
// #include "simdjson2/ppc64/begin.h"
#elif SIMDJSON2_IMPLEMENTATION_LSX
#include "simdjson2/lsx/begin.h"
#elif SIMDJSON2_IMPLEMENTATION_LASX
#include "simdjson2/lasx/begin.h"
#elif SIMDJSON2_IMPLEMENTATION_FALLBACK
#include "simdjson2/fallback/begin.h"
#else
#error "All possible implementations (including fallback) have been disabled! simdjson2 will not run."
#endif
#endif // SIMDJSON2_IMPLEMENTATION
#endif // SIMDJSON2_CONDITIONAL_INCLUDE

namespace simdjson2 {
namespace SIMDJSON2_IMPLEMENTATION {

struct open_container;
class dom_parser_implementation;

/**
 * The type of a JSON number
 */
enum class number_type {
    floating_point_number=1, /// a binary64 number
    signed_integer,          /// a signed integer that fits in a 64-bit word using two's complement
    unsigned_integer,        /// a positive integer larger or equal to 1<<63
    big_integer              /// a big integer that does not fit in a 64-bit word
};

} // namespace SIMDJSON2_IMPLEMENTATION
} // namespace simdjson2

#endif // SIMDJSON2_GENERIC_BASE_H
