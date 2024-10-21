#ifndef SIMDJSON2_JSONIOUTIL_H
#define SIMDJSON2_JSONIOUTIL_H

#include "simdjson2/base.h"
#include "simdjson2/padded_string.h"

#include "simdjson2/padded_string-inl.h"

namespace simdjson2 {

#if SIMDJSON2_EXCEPTIONS
#ifndef SIMDJSON2_DISABLE_DEPRECATED_API
[[deprecated("Use padded_string::load() instead")]]
inline padded_string get_corpus(const char *path) {
  return padded_string::load(path);
}
#endif // SIMDJSON2_DISABLE_DEPRECATED_API
#endif // SIMDJSON2_EXCEPTIONS

} // namespace simdjson2

#endif // SIMDJSON2_JSONIOUTIL_H
