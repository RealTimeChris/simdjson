#ifndef SIMDJSON2_INTERNAL_SIMDPRUNE_TABLES_H
#define SIMDJSON2_INTERNAL_SIMDPRUNE_TABLES_H

#include "simdjson2/base.h"

#include <cstdint>

namespace simdjson2 { // table modified and copied from
namespace internal { // http://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetTable

extern SIMDJSON2_DLLIMPORTEXPORT const unsigned char BitsSetTable256mul2[256];

extern SIMDJSON2_DLLIMPORTEXPORT const uint8_t pshufb_combine_table[272];

// 256 * 8 bytes = 2kB, easily fits in cache.
extern SIMDJSON2_DLLIMPORTEXPORT const uint64_t thintable_epi8[256];

} // namespace internal
} // namespace simdjson2

#endif // SIMDJSON2_INTERNAL_SIMDPRUNE_TABLES_H
