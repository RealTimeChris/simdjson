#ifndef SIMDJSON2_COMPILER_CHECK_H
#define SIMDJSON2_COMPILER_CHECK_H

#ifndef __cplusplus
#error simdjson2 requires a C++ compiler
#endif

#ifndef SIMDJSON2_CPLUSPLUS
#if defined(_MSVC_LANG) && !defined(__clang__)
#define SIMDJSON2_CPLUSPLUS (_MSC_VER == 1900 ? 201103L : _MSVC_LANG)
#else
#define SIMDJSON2_CPLUSPLUS __cplusplus
#endif
#endif

// C++ 23
#if !defined(SIMDJSON2_CPLUSPLUS23) && (SIMDJSON2_CPLUSPLUS >= 202302L)
#define SIMDJSON2_CPLUSPLUS23 1
#endif

// C++ 20
#if !defined(SIMDJSON2_CPLUSPLUS20) && (SIMDJSON2_CPLUSPLUS >= 202002L)
#define SIMDJSON2_CPLUSPLUS20 1
#endif

// C++ 17
#if !defined(SIMDJSON2_CPLUSPLUS17) && (SIMDJSON2_CPLUSPLUS >= 201703L)
#define SIMDJSON2_CPLUSPLUS17 1
#endif

// C++ 14
#if !defined(SIMDJSON2_CPLUSPLUS14) && (SIMDJSON2_CPLUSPLUS >= 201402L)
#define SIMDJSON2_CPLUSPLUS14 1
#endif

// C++ 11
#if !defined(SIMDJSON2_CPLUSPLUS11) && (SIMDJSON2_CPLUSPLUS >= 201103L)
#define SIMDJSON2_CPLUSPLUS11 1
#endif

#ifndef SIMDJSON2_CPLUSPLUS11
#error simdjson2 requires a compiler compliant with the C++11 standard
#endif

#ifndef SIMDJSON2_IF_CONSTEXPR
#if SIMDJSON2_CPLUSPLUS17
#define SIMDJSON2_IF_CONSTEXPR if constexpr
#else
#define SIMDJSON2_IF_CONSTEXPR if
#endif
#endif

#ifdef __has_include
#if __has_include(<version>)
#include <version>
#endif
#endif

#ifdef __cpp_concepts
#include <utility>
#define SIMDJSON2_SUPPORTS_DESERIALIZATION 1
#else // __cpp_concepts
#define SIMDJSON2_SUPPORTS_DESERIALIZATION 0
#endif
#endif // SIMDJSON2_COMPILER_CHECK_H
