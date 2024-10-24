#ifndef SIMDJSON2_INTERNAL_TAPE_TYPE_H
#define SIMDJSON2_INTERNAL_TAPE_TYPE_H

namespace simdjson2 {
namespace internal {

/**
 * The possible types in the tape.
 */
enum class tape_type {
  ROOT = 'r',
  START_ARRAY = '[',
  START_OBJECT = '{',
  END_ARRAY = ']',
  END_OBJECT = '}',
  STRING = '"',
  INT64 = 'l',
  UINT64 = 'u',
  DOUBLE = 'd',
  TRUE_VALUE = 't',
  FALSE_VALUE = 'f',
  NULL_VALUE = 'n'
}; // enum class tape_type

} // namespace internal
} // namespace simdjson2

#endif // SIMDJSON2_INTERNAL_TAPE_TYPE_H
