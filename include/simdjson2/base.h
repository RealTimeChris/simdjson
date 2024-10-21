/**
 * @file Base declarations for all simdjson2 headers
 * @private
 */
#ifndef SIMDJSON2_BASE_H
#define SIMDJSON2_BASE_H

#include "simdjson2/common_defs.h"
#include "simdjson2/compiler_check.h"
#include "simdjson2/error.h"
#include "simdjson2/portability.h"
#include "simdjson2/concepts.h"

/**
 * @brief The top level simdjson2 namespace, containing everything the library provides.
 */
namespace simdjson2 {

SIMDJSON2_PUSH_DISABLE_UNUSED_WARNINGS

/** The maximum document size supported by simdjson2. */
constexpr size_t SIMDJSON2_MAXSIZE_BYTES = 0xFFFFFFFF;

/**
 * The amount of padding needed in a buffer to parse JSON.
 *
 * The input buf should be readable up to buf + SIMDJSON2_PADDING
 * this is a stopgap; there should be a better description of the
 * main loop and its behavior that abstracts over this
 * See https://github.com/simdjson2/simdjson2/issues/174
 */
constexpr size_t SIMDJSON2_PADDING = 256;

/**
 * By default, simdjson2 supports this many nested objects and arrays.
 *
 * This is the default for parser::max_depth().
 */
constexpr size_t DEFAULT_MAX_DEPTH = 1024;

SIMDJSON2_POP_DISABLE_UNUSED_WARNINGS

class implementation;
struct padded_string;
class padded_string_view;
enum class stage1_mode;

namespace internal {

template<typename T>
class atomic_ptr;
class dom_parser_implementation;
class escape_json_string;
class tape_ref;
struct value128;
enum class tape_type;

} // namespace internal
} // namespace simdjson2

#endif // SIMDJSON2_BASE_H
