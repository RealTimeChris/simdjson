#ifndef SIMDJSON2_GENERIC_ONDEMAND_BASE_H

#ifndef SIMDJSON2_CONDITIONAL_INCLUDE
#define SIMDJSON2_GENERIC_ONDEMAND_BASE_H
#include "simdjson2/generic/base.h"
#endif // SIMDJSON2_CONDITIONAL_INCLUDE

namespace simdjson2 {
namespace SIMDJSON2_IMPLEMENTATION {
/**
 * A fast, simple, DOM-like interface that parses JSON as you use it.
 *
 * Designed for maximum speed and a lower memory profile.
 */
namespace ondemand {

/** Represents the depth of a JSON value (number of nested arrays/objects). */
using depth_t = int32_t;

/** @copydoc simdjson2::SIMDJSON2_IMPLEMENTATION::number_type */
using number_type = simdjson2::SIMDJSON2_IMPLEMENTATION::number_type;

/** @private Position in the JSON buffer indexes */
using token_position = const uint32_t *;

class array;
class array_iterator;
class document;
class document_reference;
class document_stream;
class field;
class json_iterator;
enum class json_type;
struct number;
class object;
class object_iterator;
class parser;
class raw_json_string;
class token_iterator;
class value;
class value_iterator;

} // namespace ondemand
} // namespace SIMDJSON2_IMPLEMENTATION
} // namespace simdjson2

#endif // SIMDJSON2_GENERIC_ONDEMAND_BASE_H