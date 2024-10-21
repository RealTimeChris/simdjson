#ifndef SIMDJSON2_GENERIC_ONDEMAND_SERIALIZATION_H

#ifndef SIMDJSON2_CONDITIONAL_INCLUDE
#define SIMDJSON2_GENERIC_ONDEMAND_SERIALIZATION_H
#include "simdjson2/generic/ondemand/base.h"
#endif // SIMDJSON2_CONDITIONAL_INCLUDE

namespace simdjson2 {
/**
 * Create a string-view instance out of a document instance. The string-view instance
 * contains JSON text that is suitable to be parsed as JSON again. It does not
 * validate the content.
 */
inline simdjson2_result<std::string_view> to_json_string(SIMDJSON2_IMPLEMENTATION::ondemand::document& x) noexcept;
/**
 * Create a string-view instance out of a value instance. The string-view instance
 * contains JSON text that is suitable to be parsed as JSON again. The value must
 * not have been accessed previously. It does not
 * validate the content.
 */
inline simdjson2_result<std::string_view> to_json_string(SIMDJSON2_IMPLEMENTATION::ondemand::value& x) noexcept;
/**
 * Create a string-view instance out of an object instance. The string-view instance
 * contains JSON text that is suitable to be parsed as JSON again. It does not
 * validate the content.
 */
inline simdjson2_result<std::string_view> to_json_string(SIMDJSON2_IMPLEMENTATION::ondemand::object& x) noexcept;
/**
 * Create a string-view instance out of an array instance. The string-view instance
 * contains JSON text that is suitable to be parsed as JSON again. It does not
 * validate the content.
 */
inline simdjson2_result<std::string_view> to_json_string(SIMDJSON2_IMPLEMENTATION::ondemand::array& x) noexcept;
inline simdjson2_result<std::string_view> to_json_string(simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::document> x);
inline simdjson2_result<std::string_view> to_json_string(simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::value> x);
inline simdjson2_result<std::string_view> to_json_string(simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::object> x);
inline simdjson2_result<std::string_view> to_json_string(simdjson2_result<SIMDJSON2_IMPLEMENTATION::ondemand::array> x);
} // namespace simdjson2

/**
 * We want to support argument-dependent lookup (ADL).
 * Hence we should define operator<< in the namespace
 * where the argument (here value, object, etc.) resides.
 * Credit: @madhur4127
 * See https://github.com/simdjson2/simdjson2/issues/1768
 */
namespace simdjson2 { namespace SIMDJSON2_IMPLEMENTATION { namespace ondemand {

/**
 * Print JSON to an output stream.  It does not
 * validate the content.
 *
 * @param out The output stream.
 * @param value The element.
 * @throw if there is an error with the underlying output stream. simdjson2 itself will not throw.
 */
inline std::ostream& operator<<(std::ostream& out, simdjson2::SIMDJSON2_IMPLEMENTATION::ondemand::value x);
#if SIMDJSON2_EXCEPTIONS
inline std::ostream& operator<<(std::ostream& out, simdjson2::simdjson2_result<simdjson2::SIMDJSON2_IMPLEMENTATION::ondemand::value> x);
#endif
/**
 * Print JSON to an output stream. It does not
 * validate the content.
 *
 * @param out The output stream.
 * @param value The array.
 * @throw if there is an error with the underlying output stream. simdjson2 itself will not throw.
 */
inline std::ostream& operator<<(std::ostream& out, simdjson2::SIMDJSON2_IMPLEMENTATION::ondemand::array value);
#if SIMDJSON2_EXCEPTIONS
inline std::ostream& operator<<(std::ostream& out, simdjson2::simdjson2_result<simdjson2::SIMDJSON2_IMPLEMENTATION::ondemand::array> x);
#endif
/**
 * Print JSON to an output stream. It does not
 * validate the content.
 *
 * @param out The output stream.
 * @param value The array.
 * @throw if there is an error with the underlying output stream. simdjson2 itself will not throw.
 */
inline std::ostream& operator<<(std::ostream& out, simdjson2::SIMDJSON2_IMPLEMENTATION::ondemand::document& value);
#if SIMDJSON2_EXCEPTIONS
inline std::ostream& operator<<(std::ostream& out, simdjson2::simdjson2_result<simdjson2::SIMDJSON2_IMPLEMENTATION::ondemand::document>&& x);
#endif
inline std::ostream& operator<<(std::ostream& out, simdjson2::SIMDJSON2_IMPLEMENTATION::ondemand::document_reference& value);
#if SIMDJSON2_EXCEPTIONS
inline std::ostream& operator<<(std::ostream& out, simdjson2::simdjson2_result<simdjson2::SIMDJSON2_IMPLEMENTATION::ondemand::document_reference>&& x);
#endif
/**
 * Print JSON to an output stream. It does not
 * validate the content.
 *
 * @param out The output stream.
 * @param value The object.
 * @throw if there is an error with the underlying output stream. simdjson2 itself will not throw.
 */
inline std::ostream& operator<<(std::ostream& out, simdjson2::SIMDJSON2_IMPLEMENTATION::ondemand::object value);
#if SIMDJSON2_EXCEPTIONS
inline std::ostream& operator<<(std::ostream& out, simdjson2::simdjson2_result<simdjson2::SIMDJSON2_IMPLEMENTATION::ondemand::object> x);
#endif
}}} // namespace simdjson2::SIMDJSON2_IMPLEMENTATION::ondemand

#endif // SIMDJSON2_GENERIC_ONDEMAND_SERIALIZATION_H