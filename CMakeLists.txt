cmake_minimum_required(VERSION 3.14)

project(
    simdjson2
    # The version number is modified by tools/release.py
    VERSION 3.10.1
    DESCRIPTION "Parsing gigabytes of JSON per second"
    HOMEPAGE_URL "https://simdjson2.org/"
    LANGUAGES CXX C
)

set(SIMDJSON2_GITHUB_REPOSITORY "https://github.com/simdjson2/simdjson2")

if (NOT WIN32 AND NOT APPLE)
    # Apply -march=native for all configurations
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mavx -mavx2")
    # Optionally, you can apply it for specific configurations (like Release)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -march=native")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mavx2")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mavx")

    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -march=native")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -mavx2")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -mavx")
endif()

string(
    COMPARE EQUAL
    "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}"
    is_top_project
)

# ---- Options, variables ----

# These version numbers are modified by tools/release.py
set(SIMDJSON2_LIB_VERSION "23.0.0" CACHE STRING "simdjson2 library version")
set(SIMDJSON2_LIB_SOVERSION "23" CACHE STRING "simdjson2 library soversion")

option(SIMDJSON2_BUILD_STATIC_LIB "Build simdjson2_static library along with simdjson2 (only makes sense if BUILD_SHARED_LIBS=ON)" OFF)
if(SIMDJSON2_BUILD_STATIC_LIB AND NOT BUILD_SHARED_LIBS)
  message(WARNING "SIMDJSON2_BUILD_STATIC_LIB only makes sense if BUILD_SHARED_LIBS is set to ON")
  message(WARNING "You might be building and installing a two identical static libraries.")
endif()

option(SIMDJSON2_ENABLE_THREADS "Link with thread support" ON)

include(cmake/simdjson2-props.cmake)
include(cmake/implementation-flags.cmake)
include(cmake/exception-flags.cmake)

option(SIMDJSON2_DISABLE_DEPRECATED_API "Disables deprecated APIs" OFF)
if(SIMDJSON2_DISABLE_DEPRECATED_API)
  simdjson2_add_props(
      target_compile_definitions PUBLIC
      SIMDJSON2_DISABLE_DEPRECATED_API=1
  )
endif()

option(SIMDJSON2_DEVELOPMENT_CHECKS "Enable development-time aids, such as \
checks for incorrect API usage. Enabled by default in DEBUG." OFF)
if(SIMDJSON2_DEVELOPMENT_CHECKS)
  simdjson2_add_props(
      target_compile_definitions PUBLIC
      SIMDJSON2_DEVELOPMENT_CHECKS
  )
endif()

if(is_top_project)
  option(SIMDJSON2_DEVELOPER_MODE "Enable targets for developing simdjson2" OFF)
  option(BUILD_SHARED_LIBS "Build simdjson2 as a shared library" OFF)
  option(SIMDJSON2_SINGLEHEADER "Disable singleheader generation" ON)
endif()

include(cmake/handle-deprecations.cmake)
include(cmake/developer-options.cmake)

# ---- simdjson2 library ----

set(SIMDJSON2_SOURCES src/simdjson2.cpp)

add_library(simdjson2 ${SIMDJSON2_SOURCES})
add_library(simdjson2::simdjson2 ALIAS simdjson2)
set(SIMDJSON2_LIBRARIES simdjson2)

if(SIMDJSON2_BUILD_STATIC_LIB)
  add_library(simdjson2_static STATIC ${SIMDJSON2_SOURCES})
  add_library(simdjson2::simdjson2_static ALIAS simdjson2_static)
  list(APPEND SIMDJSON2_LIBRARIES simdjson2_static)
endif()

set_target_properties(
    simdjson2 PROPERTIES
    VERSION "${SIMDJSON2_LIB_VERSION}"
    SOVERSION "${SIMDJSON2_LIB_SOVERSION}"
    # FIXME: symbols should be hidden by default
    WINDOWS_EXPORT_ALL_SYMBOLS YES
)

# FIXME: Use proper CMake integration for exports
if(MSVC AND BUILD_SHARED_LIBS)
  target_compile_definitions(
      simdjson2
      PRIVATE SIMDJSON2_BUILDING_WINDOWS_DYNAMIC_LIBRARY=1
      INTERFACE SIMDJSON2_USING_WINDOWS_DYNAMIC_LIBRARY=1
  )
endif()

simdjson2_add_props(
    target_include_directories
    PUBLIC "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
    PRIVATE "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>"
)

simdjson2_add_props(target_compile_features PUBLIC cxx_std_11)

# workaround for GNU GCC poor AVX load/store code generation
if(
    CMAKE_CXX_COMPILER_ID STREQUAL "GNU"
    AND CMAKE_SYSTEM_PROCESSOR MATCHES "^(i.86|x86(_64)?)$"
)
  simdjson2_add_props(
      target_compile_options PRIVATE
      -mno-avx256-split-unaligned-load -mno-avx256-split-unaligned-store
  )
endif()

if(CMAKE_SYSTEM_PROCESSOR MATCHES "^(loongarch64)$")
    option(SIMDJSON2_PREFER_LSX "Prefer LoongArch SX" ON)
    include(CheckCXXCompilerFlag)
    check_cxx_compiler_flag(-mlasx COMPILER_SUPPORTS_LASX)
    check_cxx_compiler_flag(-mlsx COMPILER_SUPPORTS_LSX)
    if(COMPILER_SUPPORTS_LASX AND NOT SIMDJSON2_PREFER_LSX)
        simdjson2_add_props(
            target_compile_options PRIVATE
            -mlasx
        )
    elseif(COMPILER_SUPPORTS_LSX)
        simdjson2_add_props(
            target_compile_options PRIVATE
            -mlsx
        )
    endif()
endif()

# GCC and Clang have horrendous Debug builds when using SIMD.
# A common fix is to use '-Og' instead.
# bug https://gcc.gnu.org/bugzilla/show_bug.cgi?id=54412
if(
    (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR
        CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR
        CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
)
  message(STATUS "Adding -Og to compile flag")
  simdjson2_add_props(
      target_compile_options PRIVATE
      $<$<CONFIG:DEBUG>:-Og>
  )
endif()

if(SIMDJSON2_ENABLE_THREADS)
  find_package(Threads REQUIRED)
  simdjson2_add_props(target_link_libraries PUBLIC Threads::Threads)
  simdjson2_add_props(target_compile_definitions PUBLIC SIMDJSON2_THREADS_ENABLED=1)
endif()

simdjson2_apply_props(simdjson2)
if(SIMDJSON2_BUILD_STATIC_LIB)
  simdjson2_apply_props(simdjson2_static)
endif()

# ---- Install rules ----

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

if(SIMDJSON2_SINGLEHEADER)
  install(
      FILES singleheader/simdjson2.h
      DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
      COMPONENT simdjson2_Development
  )
endif()
set(CXX_COM)

install(
    TARGETS simdjson2
    EXPORT simdjson2Targets
    RUNTIME COMPONENT simdjson2_Runtime
    LIBRARY COMPONENT simdjson2_Runtime
    NAMELINK_COMPONENT simdjson2_Development
    ARCHIVE COMPONENT simdjson2_Development
    INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)
configure_file(cmake/simdjson2-config.cmake.in simdjson2-config.cmake @ONLY)

write_basic_package_version_file(
    simdjson2-config-version.cmake
    COMPATIBILITY SameMinorVersion
)

set(
    SIMDJSON2_INSTALL_CMAKEDIR "${CMAKE_INSTALL_LIBDIR}/cmake/simdjson2"
    CACHE STRING "CMake package config location relative to the install prefix"
)
mark_as_advanced(SIMDJSON2_INSTALL_CMAKEDIR)

install(
    FILES
    "${PROJECT_BINARY_DIR}/simdjson2-config.cmake"
    "${PROJECT_BINARY_DIR}/simdjson2-config-version.cmake"
    DESTINATION "${SIMDJSON2_INSTALL_CMAKEDIR}"
    COMPONENT simdjson2_Development
)

install(
    EXPORT simdjson2Targets
    NAMESPACE simdjson2::
    DESTINATION "${SIMDJSON2_INSTALL_CMAKEDIR}"
    COMPONENT simdjson2_Development
)

if(SIMDJSON2_BUILD_STATIC_LIB)
  install(
      TARGETS simdjson2_static
      EXPORT simdjson2_staticTargets
      ARCHIVE COMPONENT simdjson2_Development
      INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
  )
  install(
      EXPORT simdjson2_staticTargets
      NAMESPACE simdjson2::
      DESTINATION "${SIMDJSON2_INSTALL_CMAKEDIR}"
      COMPONENT simdjson2_Development
  )
endif()

# pkg-config
include(cmake/JoinPaths.cmake)
join_paths(PKGCONFIG_INCLUDEDIR "\${prefix}" "${CMAKE_INSTALL_INCLUDEDIR}")
join_paths(PKGCONFIG_LIBDIR "\${prefix}" "${CMAKE_INSTALL_LIBDIR}")

if(SIMDJSON2_ENABLE_THREADS)
  set(PKGCONFIG_CFLAGS "-DSIMDJSON2_THREADS_ENABLED=1")
  if(CMAKE_THREAD_LIBS_INIT)
    set(PKGCONFIG_LIBS_PRIVATE "Libs.private: ${CMAKE_THREAD_LIBS_INIT}")
  endif()
endif()

configure_file("simdjson2.pc.in" "simdjson2.pc" @ONLY)
install(
    FILES "${CMAKE_CURRENT_BINARY_DIR}/simdjson2.pc"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig"
)

#
# CPack
#
if(is_top_project)
  set(CPACK_PACKAGE_VENDOR "Daniel Lemire")
  set(CPACK_PACKAGE_CONTACT "lemire@gmail.com")
  set(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/LICENSE")
  set(CPACK_RESOURCE_FILE_README "${PROJECT_SOURCE_DIR}/README.md")

  set(CPACK_RPM_PACKAGE_LICENSE "${PROJECT_SOURCE_DIR}/LICENSE")

  set(CPACK_SOURCE_GENERATOR "TGZ;ZIP")

  include(CPack)
endif()

# ---- Developer mode extras ----

if(is_top_project AND NOT SIMDJSON2_DEVELOPER_MODE)
  message(STATUS "Building only the library. Advanced users and contributors may want to turn SIMDJSON2_DEVELOPER_MODE to ON, e.g., via -D SIMDJSON2_DEVELOPER_MODE=ON.")
elseif(SIMDJSON2_DEVELOPER_MODE AND NOT is_top_project)
  message(AUTHOR_WARNING "Developer mode in simdjson2 is intended for the developers of simdjson2")
endif()

if(NOT SIMDJSON2_DEVELOPER_MODE)
  return()
endif()

simdjson2_apply_props(simdjson2-internal-flags)

set(
    SIMDJSON2_USER_CMAKECACHE
    "${CMAKE_BINARY_DIR}/.simdjson2-user-CMakeCache.txt"
)
add_custom_target(
    simdjson2-user-cmakecache
    COMMAND "${CMAKE_COMMAND}"
    -D "BINARY_DIR=${CMAKE_BINARY_DIR}"
    -D "USER_CMAKECACHE=${SIMDJSON2_USER_CMAKECACHE}"
    -P "${PROJECT_SOURCE_DIR}/cmake/simdjson2-user-cmakecache.cmake"
    VERBATIM
)

# Setup tests
enable_testing()
# So we can build just tests with "make all_tests"
add_custom_target(all_tests)

add_subdirectory(windows)
include(cmake/CPM.cmake)
add_subdirectory(dependencies) ## This needs to be before tools because of cxxopts
add_subdirectory(tools)  ## This needs to be before tests because of cxxopts

# Data: jsonexamples is left with only the bare essential.
# most of the data has been moved to https://github.com/simdjson2/simdjson2-data
add_subdirectory(jsonexamples)

if(SIMDJSON2_SINGLEHEADER)
add_subdirectory(singleheader)
endif()



#
# Compile tools / tests / benchmarks
#
add_subdirectory(tests)
add_subdirectory(examples)
if(CMAKE_SIZEOF_VOID_P EQUAL 8) # we only include the benchmarks on 64-bit systems.
  add_subdirectory(benchmark)
endif()
add_subdirectory(fuzz)

#
# Source files should be just ASCII
#
find_program(FIND find)
find_program(FILE file)
find_program(GREP grep)
if(FIND AND FILE AND GREP)
  add_test(
      NAME just_ascii
      COMMAND sh -c "\
${FIND} include src windows tools singleheader tests examples benchmark \
-path benchmark/checkperf-reference -prune -name '*.h' -o -name '*.cpp' \
-type f -exec ${FILE} '{}' \; | ${GREP} -qv ASCII || exit 0  && exit 1"
      WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
  )
endif()

##
## In systems like R, libraries must not use stderr or abort to be acceptable.
## Thus we make it a hard rule that one is not allowed to call abort or stderr.
## The sanitized builds are allowed to abort.
##
if(NOT SIMDJSON2_SANITIZE)
  find_program(GREP grep)
  find_program(NM nm)
  if((NOT GREP) OR (NOT NM))
    message("grep and nm are unavailable on this system.")
  else()
    add_test(
      NAME "avoid_abort"
      # Under FreeBSD, the __cxa_guard_abort symbol may appear but it is fine.
      # So we want to look for <space><possibly _>abort as a test.
      COMMAND sh -c "${NM}  $<TARGET_FILE_NAME:simdjson2> |  ${GREP} ' _*abort' || exit 0  && exit 1"
      WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
    )
    add_test(
      NAME "avoid_cout"
      COMMAND sh -c "${NM}  $<TARGET_FILE_NAME:simdjson2> |  ${GREP} ' _*cout' || exit 0  && exit 1"
      WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
    )
    add_test(
      NAME "avoid_cerr"
      COMMAND sh -c "${NM}  $<TARGET_FILE_NAME:simdjson2> |  ${GREP} ' _*cerr' || exit 0  && exit 1"
      WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
    )
    add_test(
      NAME "avoid_printf"
      COMMAND sh -c "${NM}  $<TARGET_FILE_NAME:simdjson2> |  ${GREP} ' _*printf' || exit 0  && exit 1"
      WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
    )
    add_test(
      NAME "avoid_stdout"
      COMMAND sh -c "${NM}  $<TARGET_FILE_NAME:simdjson2> |  ${GREP} stdout || exit 0 && exit 1"
      WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
    )
    add_test(
      NAME "avoid_stderr"
      COMMAND sh -c "${NM}  $<TARGET_FILE_NAME:simdjson2> |  ${GREP} stderr || exit 0 && exit 1"
      WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
    )
  endif()
endif()
