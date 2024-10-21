#if defined(SIMDJSON2_CONDITIONAL_INCLUDE) && !defined(SIMDJSON2_GENERIC_ONDEMAND_DEPENDENCIES_H)
#error simdjson2/generic/ondemand/dependencies.h must be included before simdjson2/generic/ondemand/amalgamated.h!
#endif

// Stuff other things depend on
#include "simdjson2/generic/ondemand/base.h"
#include "simdjson2/generic/ondemand/deserialize.h"
#include "simdjson2/generic/ondemand/value_iterator.h"
#include "simdjson2/generic/ondemand/value.h"
#include "simdjson2/generic/ondemand/logger.h"
#include "simdjson2/generic/ondemand/token_iterator.h"
#include "simdjson2/generic/ondemand/json_iterator.h"
#include "simdjson2/generic/ondemand/json_type.h"
#include "simdjson2/generic/ondemand/raw_json_string.h"
#include "simdjson2/generic/ondemand/parser.h"

// All other declarations
#include "simdjson2/generic/ondemand/array.h"
#include "simdjson2/generic/ondemand/array_iterator.h"
#include "simdjson2/generic/ondemand/document.h"
#include "simdjson2/generic/ondemand/document_stream.h"
#include "simdjson2/generic/ondemand/field.h"
#include "simdjson2/generic/ondemand/object.h"
#include "simdjson2/generic/ondemand/object_iterator.h"
#include "simdjson2/generic/ondemand/serialization.h"

// Deserialization for standard types
#include "simdjson2/generic/ondemand/std_deserialize.h"

// Inline definitions
#include "simdjson2/generic/ondemand/array-inl.h"
#include "simdjson2/generic/ondemand/array_iterator-inl.h"
#include "simdjson2/generic/ondemand/value-inl.h"
#include "simdjson2/generic/ondemand/document-inl.h"
#include "simdjson2/generic/ondemand/document_stream-inl.h"
#include "simdjson2/generic/ondemand/field-inl.h"
#include "simdjson2/generic/ondemand/json_iterator-inl.h"
#include "simdjson2/generic/ondemand/json_type-inl.h"
#include "simdjson2/generic/ondemand/logger-inl.h"
#include "simdjson2/generic/ondemand/object-inl.h"
#include "simdjson2/generic/ondemand/object_iterator-inl.h"
#include "simdjson2/generic/ondemand/parser-inl.h"
#include "simdjson2/generic/ondemand/raw_json_string-inl.h"
#include "simdjson2/generic/ondemand/serialization-inl.h"
#include "simdjson2/generic/ondemand/token_iterator-inl.h"
#include "simdjson2/generic/ondemand/value_iterator-inl.h"


