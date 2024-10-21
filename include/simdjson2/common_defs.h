#ifndef SIMDJSON2_COMMON_DEFS_H
#define SIMDJSON2_COMMON_DEFS_H

#include <cassert>
#include "simdjson2/compiler_check.h"
#include "simdjson2/portability.h"

namespace simdjson2 {
namespace internal {
/**
 * @private
 * Our own implementation of the C++17 to_chars function.
 * Defined in src/to_chars
 */
char *to_chars(char *first, const char *last, double value);
/**
 * @private
 * A number parsing routine.
 * Defined in src/from_chars
 */
double from_chars(const char *first) noexcept;
double from_chars(const char *first, const char* end) noexcept;
}

#ifndef SIMDJSON2_EXCEPTIONS
#if __cpp_exceptions
#define SIMDJSON2_EXCEPTIONS 1
#else
#define SIMDJSON2_EXCEPTIONS 0
#endif
#endif

} // namespace simdjson2

#if defined(__GNUC__)
  // Marks a block with a name so that MCA analysis can see it.
  #define SIMDJSON2_BEGIN_DEBUG_BLOCK(name) __asm volatile("# LLVM-MCA-BEGIN " #name);
  #define SIMDJSON2_END_DEBUG_BLOCK(name) __asm volatile("# LLVM-MCA-END " #name);
  #define SIMDJSON2_DEBUG_BLOCK(name, block) BEGIN_DEBUG_BLOCK(name); block; END_DEBUG_BLOCK(name);
#else
  #define SIMDJSON2_BEGIN_DEBUG_BLOCK(name)
  #define SIMDJSON2_END_DEBUG_BLOCK(name)
  #define SIMDJSON2_DEBUG_BLOCK(name, block)
#endif

// Align to N-byte boundary
#define SIMDJSON2_ROUNDUP_N(a, n) (((a) + ((n)-1)) & ~((n)-1))
#define SIMDJSON2_ROUNDDOWN_N(a, n) ((a) & ~((n)-1))

#define SIMDJSON2_ISALIGNED_N(ptr, n) (((uintptr_t)(ptr) & ((n)-1)) == 0)

#if SIMDJSON2_REGULAR_VISUAL_STUDIO
  // We could use [[deprecated]] but it requires C++14
  #define simdjson2_deprecated __declspec(deprecated)

  #define simdjson2_really_inline __forceinline
  #define simdjson2_never_inline __declspec(noinline)

  #define simdjson2_unused
  #define simdjson2_warn_unused

  #ifndef simdjson2_likely
  #define simdjson2_likely(x) x
  #endif
  #ifndef simdjson2_unlikely
  #define simdjson2_unlikely(x) x
  #endif

  #define SIMDJSON2_PUSH_DISABLE_WARNINGS __pragma(warning( push ))
  #define SIMDJSON2_PUSH_DISABLE_ALL_WARNINGS __pragma(warning( push, 0 ))
  #define SIMDJSON2_DISABLE_VS_WARNING(WARNING_NUMBER) __pragma(warning( disable : WARNING_NUMBER ))
  // Get rid of Intellisense-only warnings (Code Analysis)
  // Though __has_include is C++17, it is supported in Visual Studio 2017 or better (_MSC_VER>=1910).
  #ifdef __has_include
  #if __has_include(<CppCoreCheck\Warnings.h>)
  #include <CppCoreCheck\Warnings.h>
  #define SIMDJSON2_DISABLE_UNDESIRED_WARNINGS SIMDJSON2_DISABLE_VS_WARNING(ALL_CPPCORECHECK_WARNINGS)
  #endif
  #endif

  #ifndef SIMDJSON2_DISABLE_UNDESIRED_WARNINGS
  #define SIMDJSON2_DISABLE_UNDESIRED_WARNINGS
  #endif

  #define SIMDJSON2_DISABLE_DEPRECATED_WARNING SIMDJSON2_DISABLE_VS_WARNING(4996)
  #define SIMDJSON2_DISABLE_STRICT_OVERFLOW_WARNING
  #define SIMDJSON2_POP_DISABLE_WARNINGS __pragma(warning( pop ))

  #define SIMDJSON2_PUSH_DISABLE_UNUSED_WARNINGS
  #define SIMDJSON2_POP_DISABLE_UNUSED_WARNINGS

#else // SIMDJSON2_REGULAR_VISUAL_STUDIO
  // We could use [[deprecated]] but it requires C++14
  #define simdjson2_deprecated __attribute__((deprecated))

  #define simdjson2_really_inline inline __attribute__((always_inline))
  #define simdjson2_never_inline inline __attribute__((noinline))

  #define simdjson2_unused __attribute__((unused))
  #define simdjson2_warn_unused __attribute__((warn_unused_result))

  #ifndef simdjson2_likely
  #define simdjson2_likely(x) __builtin_expect(!!(x), 1)
  #endif
  #ifndef simdjson2_unlikely
  #define simdjson2_unlikely(x) __builtin_expect(!!(x), 0)
  #endif

  #define SIMDJSON2_PUSH_DISABLE_WARNINGS _Pragma("GCC diagnostic push")
  // gcc doesn't seem to disable all warnings with all and extra, add warnings here as necessary
  // We do it separately for clang since it has different warnings.
  #ifdef __clang__
  // clang is missing -Wmaybe-uninitialized.
  #define SIMDJSON2_PUSH_DISABLE_ALL_WARNINGS SIMDJSON2_PUSH_DISABLE_WARNINGS \
    SIMDJSON2_DISABLE_GCC_WARNING(-Weffc++) \
    SIMDJSON2_DISABLE_GCC_WARNING(-Wall) \
    SIMDJSON2_DISABLE_GCC_WARNING(-Wconversion) \
    SIMDJSON2_DISABLE_GCC_WARNING(-Wextra) \
    SIMDJSON2_DISABLE_GCC_WARNING(-Wattributes) \
    SIMDJSON2_DISABLE_GCC_WARNING(-Wimplicit-fallthrough) \
    SIMDJSON2_DISABLE_GCC_WARNING(-Wnon-virtual-dtor) \
    SIMDJSON2_DISABLE_GCC_WARNING(-Wreturn-type) \
    SIMDJSON2_DISABLE_GCC_WARNING(-Wshadow) \
    SIMDJSON2_DISABLE_GCC_WARNING(-Wunused-parameter) \
    SIMDJSON2_DISABLE_GCC_WARNING(-Wunused-variable)
  #else // __clang__
  #define SIMDJSON2_PUSH_DISABLE_ALL_WARNINGS SIMDJSON2_PUSH_DISABLE_WARNINGS \
    SIMDJSON2_DISABLE_GCC_WARNING(-Weffc++) \
    SIMDJSON2_DISABLE_GCC_WARNING(-Wall) \
    SIMDJSON2_DISABLE_GCC_WARNING(-Wconversion) \
    SIMDJSON2_DISABLE_GCC_WARNING(-Wextra) \
    SIMDJSON2_DISABLE_GCC_WARNING(-Wattributes) \
    SIMDJSON2_DISABLE_GCC_WARNING(-Wimplicit-fallthrough) \
    SIMDJSON2_DISABLE_GCC_WARNING(-Wnon-virtual-dtor) \
    SIMDJSON2_DISABLE_GCC_WARNING(-Wreturn-type) \
    SIMDJSON2_DISABLE_GCC_WARNING(-Wshadow) \
    SIMDJSON2_DISABLE_GCC_WARNING(-Wunused-parameter) \
    SIMDJSON2_DISABLE_GCC_WARNING(-Wunused-variable) \
    SIMDJSON2_DISABLE_GCC_WARNING(-Wmaybe-uninitialized) \
    SIMDJSON2_DISABLE_GCC_WARNING(-Wformat-security)
  #endif // __clang__

  #define SIMDJSON2_PRAGMA(P) _Pragma(#P)
  #define SIMDJSON2_DISABLE_GCC_WARNING(WARNING) SIMDJSON2_PRAGMA(GCC diagnostic ignored #WARNING)
  #if SIMDJSON2_CLANG_VISUAL_STUDIO
  #define SIMDJSON2_DISABLE_UNDESIRED_WARNINGS SIMDJSON2_DISABLE_GCC_WARNING(-Wmicrosoft-include)
  #else
  #define SIMDJSON2_DISABLE_UNDESIRED_WARNINGS
  #endif
  #define SIMDJSON2_DISABLE_DEPRECATED_WARNING SIMDJSON2_DISABLE_GCC_WARNING(-Wdeprecated-declarations)
  #define SIMDJSON2_DISABLE_STRICT_OVERFLOW_WARNING SIMDJSON2_DISABLE_GCC_WARNING(-Wstrict-overflow)
  #define SIMDJSON2_POP_DISABLE_WARNINGS _Pragma("GCC diagnostic pop")

  #define SIMDJSON2_PUSH_DISABLE_UNUSED_WARNINGS SIMDJSON2_PUSH_DISABLE_WARNINGS \
    SIMDJSON2_DISABLE_GCC_WARNING(-Wunused)
  #define SIMDJSON2_POP_DISABLE_UNUSED_WARNINGS SIMDJSON2_POP_DISABLE_WARNINGS



#endif // MSC_VER

#if defined(simdjson2_inline)
  // Prefer the user's definition of simdjson2_inline; don't define it ourselves.
#elif defined(__GNUC__) && !defined(__OPTIMIZE__)
  // If optimizations are disabled, forcing inlining can lead to significant
  // code bloat and high compile times. Don't use simdjson2_really_inline for
  // unoptimized builds.
  #define simdjson2_inline inline
#else
  // Force inlining for most simdjson2 functions.
  #define simdjson2_inline simdjson2_really_inline
#endif

#if SIMDJSON2_VISUAL_STUDIO
    /**
     * Windows users need to do some extra work when building
     * or using a dynamic library (DLL). When building, we need
     * to set SIMDJSON2_DLLIMPORTEXPORT to __declspec(dllexport).
     * When *using* the DLL, the user needs to set
     * SIMDJSON2_DLLIMPORTEXPORT __declspec(dllimport).
     *
     * Static libraries not need require such work.
     *
     * It does not matter here whether you are using
     * the regular visual studio or clang under visual
     * studio, you still need to handle these issues.
     *
     * Non-Windows systems do not have this complexity.
     */
    #if SIMDJSON2_BUILDING_WINDOWS_DYNAMIC_LIBRARY
    // We set SIMDJSON2_BUILDING_WINDOWS_DYNAMIC_LIBRARY when we build a DLL under Windows.
    // It should never happen that both SIMDJSON2_BUILDING_WINDOWS_DYNAMIC_LIBRARY and
    // SIMDJSON2_USING_WINDOWS_DYNAMIC_LIBRARY are set.
    #define SIMDJSON2_DLLIMPORTEXPORT __declspec(dllexport)
    #elif SIMDJSON2_USING_WINDOWS_DYNAMIC_LIBRARY
    // Windows user who call a dynamic library should set SIMDJSON2_USING_WINDOWS_DYNAMIC_LIBRARY to 1.
    #define SIMDJSON2_DLLIMPORTEXPORT __declspec(dllimport)
    #else
    // We assume by default static linkage
    #define SIMDJSON2_DLLIMPORTEXPORT
    #endif

/**
 * Workaround for the vcpkg package manager. Only vcpkg should
 * ever touch the next line. The SIMDJSON2_USING_LIBRARY macro is otherwise unused.
 */
#if SIMDJSON2_USING_LIBRARY
#define SIMDJSON2_DLLIMPORTEXPORT __declspec(dllimport)
#endif
/**
 * End of workaround for the vcpkg package manager.
 */
#else
    #define SIMDJSON2_DLLIMPORTEXPORT
#endif

// C++17 requires string_view.
#if SIMDJSON2_CPLUSPLUS17
#define SIMDJSON2_HAS_STRING_VIEW
#include <string_view> // by the standard, this has to be safe.
#endif

// This macro (__cpp_lib_string_view) has to be defined
// for C++17 and better, but if it is otherwise defined,
// we are going to assume that string_view is available
// even if we do not have C++17 support.
#ifdef __cpp_lib_string_view
#define SIMDJSON2_HAS_STRING_VIEW
#endif

// Some systems have string_view even if we do not have C++17 support,
// and even if __cpp_lib_string_view is undefined, it is the case
// with Apple clang version 11.
// We must handle it. *This is important.*
#ifndef SIMDJSON2_HAS_STRING_VIEW
#if defined __has_include
// do not combine the next #if with the previous one (unsafe)
#if __has_include (<string_view>)
// now it is safe to trigger the include
#include <string_view> // though the file is there, it does not follow that we got the implementation
#if defined(_LIBCPP_STRING_VIEW)
// Ah! So we under libc++ which under its Library Fundamentals Technical Specification, which preceded C++17,
// included string_view.
// This means that we have string_view *even though* we may not have C++17.
#define SIMDJSON2_HAS_STRING_VIEW
#endif // _LIBCPP_STRING_VIEW
#endif // __has_include (<string_view>)
#endif // defined __has_include
#endif // def SIMDJSON2_HAS_STRING_VIEW
// end of complicated but important routine to try to detect string_view.

//
// Backfill std::string_view using nonstd::string_view on systems where
// we expect that string_view is missing. Important: if we get this wrong,
// we will end up with two string_view definitions and potential trouble.
// That is why we work so hard above to avoid it.
//
#ifndef SIMDJSON2_HAS_STRING_VIEW
SIMDJSON2_PUSH_DISABLE_ALL_WARNINGS
#include "simdjson2/nonstd/string_view.hpp"
SIMDJSON2_POP_DISABLE_WARNINGS

namespace std {
  using string_view = nonstd::string_view;
}
#endif // SIMDJSON2_HAS_STRING_VIEW
#undef SIMDJSON2_HAS_STRING_VIEW // We are not going to need this macro anymore.

/// If EXPR is an error, returns it.
#define SIMDJSON2_TRY(EXPR) { auto _err = (EXPR); if (_err) { return _err; } }

// Unless the programmer has already set SIMDJSON2_DEVELOPMENT_CHECKS,
// we want to set it under debug builds. We detect a debug build
// under Visual Studio when the _DEBUG macro is set. Under the other
// compilers, we use the fact that they define __OPTIMIZE__ whenever
// they allow optimizations.
// It is possible that this could miss some cases where SIMDJSON2_DEVELOPMENT_CHECKS
// is helpful, but the programmer can set the macro SIMDJSON2_DEVELOPMENT_CHECKS.
// It could also wrongly set SIMDJSON2_DEVELOPMENT_CHECKS (e.g., if the programmer
// sets _DEBUG in a release build under Visual Studio, or if some compiler fails to
// set the __OPTIMIZE__ macro).
#ifndef SIMDJSON2_DEVELOPMENT_CHECKS
#ifdef _MSC_VER
// Visual Studio seems to set _DEBUG for debug builds.
#ifdef _DEBUG
#define SIMDJSON2_DEVELOPMENT_CHECKS 1
#endif // _DEBUG
#else // _MSC_VER
// All other compilers appear to set __OPTIMIZE__ to a positive integer
// when the compiler is optimizing.
#ifndef __OPTIMIZE__
#define SIMDJSON2_DEVELOPMENT_CHECKS 1
#endif // __OPTIMIZE__
#endif // _MSC_VER
#endif // SIMDJSON2_DEVELOPMENT_CHECKS

// The SIMDJSON2_CHECK_EOF macro is a feature flag for the "don't require padding"
// feature.

#if SIMDJSON2_CPLUSPLUS17
// if we have C++, then fallthrough is a default attribute
# define simdjson2_fallthrough [[fallthrough]]
// check if we have __attribute__ support
#elif defined(__has_attribute)
// check if we have the __fallthrough__ attribute
#if __has_attribute(__fallthrough__)
// we are good to go:
# define simdjson2_fallthrough                    __attribute__((__fallthrough__))
#endif // __has_attribute(__fallthrough__)
#endif // SIMDJSON2_CPLUSPLUS17
// on some systems, we simply do not have support for fallthrough, so use a default:
#ifndef simdjson2_fallthrough
# define simdjson2_fallthrough do {} while (0)  /* fallthrough */
#endif // simdjson2_fallthrough

#if SIMDJSON2_DEVELOPMENT_CHECKS
#define SIMDJSON2_DEVELOPMENT_ASSERT(expr) do { assert ((expr)); } while (0)
#else
#define SIMDJSON2_DEVELOPMENT_ASSERT(expr) do { } while (0)
#endif

#ifndef SIMDJSON2_UTF8VALIDATION
#define SIMDJSON2_UTF8VALIDATION 1
#endif

#ifdef __has_include
// How do we detect that a compiler supports vbmi2?
// For sure if the following header is found, we are ok?
#if __has_include(<avx512vbmi2intrin.h>)
#define SIMDJSON2_COMPILER_SUPPORTS_VBMI2 1
#endif
#endif

#ifdef _MSC_VER
#if _MSC_VER >= 1920
// Visual Studio 2019 and up support VBMI2 under x64 even if the header
// avx512vbmi2intrin.h is not found.
#define SIMDJSON2_COMPILER_SUPPORTS_VBMI2 1
#endif
#endif

// By default, we allow AVX512.
#ifndef SIMDJSON2_AVX512_ALLOWED
#define SIMDJSON2_AVX512_ALLOWED 1
#endif

#endif // SIMDJSON2_COMMON_DEFS_H
