#
# Amalgamation
#

set(SINGLEHEADER_FILES
  ${CMAKE_CURRENT_BINARY_DIR}/simdjson2.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/simdjson2.h
  ${CMAKE_CURRENT_BINARY_DIR}/amalgamate_demo.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/README.md
)
set(SINGLEHEADER_REPO_FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/simdjson2.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/simdjson2.h
  ${CMAKE_CURRENT_SOURCE_DIR}/amalgamate_demo.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/README.md
)
set_source_files_properties(${SINGLEHEADER_FILES} PROPERTIES GENERATED TRUE)

find_package(Python3 COMPONENTS Interpreter)

if (Python3_Interpreter_FOUND AND (NOT WIN32))
  add_custom_command(
    OUTPUT ${SINGLEHEADER_FILES}
    COMMAND ${CMAKE_COMMAND} -E env
      AMALGAMATE_SOURCE_PATH=${PROJECT_SOURCE_DIR}/src
      AMALGAMATE_INPUT_PATH=${PROJECT_SOURCE_DIR}/include
      AMALGAMATE_OUTPUT_PATH=${CMAKE_CURRENT_BINARY_DIR}
      ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/amalgamate.py
      #
      # This is the best way I could find to make amalgamation trigger whenever source files or
      # header files change: since the "simdjson2" library has to get rebuilt when that happens, we
      # take a dependency on the generated library file (even though we're not using it). Depending
      # on simdjson2-source doesn't do the trick because DEPENDS here can only depend on an
      # *artifact*--it won't scan source and include files the way a concrete library or executable
      # will.
      #
      # It sucks that we have to build the actual library to make it happen, but it's better than\
      # nothing!
      #
      DEPENDS amalgamate.py simdjson2
  )

  ##
  # This is used by "make amalgamate" to update the original source files.
  # You can invoke it as cmake --build . --target amalgamate
  # We obviously don't do
  # this if source and generated files are in the same place--cmake gets mad!
  if (NOT (${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_BINARY_DIR}))
    add_custom_target(amalgamate)
    add_custom_command(TARGET amalgamate
      # We don't want CMake to know that it is writing to the source directory. No magic
      # file regeneration in the source directory without the user's knowledge.
      # OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/simdjson2.cpp ${CMAKE_CURRENT_SOURCE_DIR}/simdjson2.h ${CMAKE_CURRENT_SOURCE_DIR}/amalgamate_demo.cpp ${CMAKE_CURRENT_SOURCE_DIR}/README.md
      COMMAND ${CMAKE_COMMAND} -E copy ${SINGLEHEADER_FILES} ${CMAKE_CURRENT_SOURCE_DIR}
      DEPENDS ${SINGLEHEADER_FILES}
    )
  endif()


  ##
  # Adding the ability for CMake to modify the source is problematic. In particular, it will
  # happily regenerate the source files that are missing, silently. We do not want to do this.
  # If they are missing source files, the build should fail. You should not get silent patching
  # by CMake. The user can easily regenerate the files, deliberately.
  #
  # DO NOT DO THIS:
  # add_custom_target(amalgamate DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/simdjson2.cpp ${CMAKE_CURRENT_SOURCE_DIR}/simdjson2.h ${CMAKE_CURRENT_SOURCE_DIR}/amalgamate_demo.cpp ${CMAKE_CURRENT_SOURCE_DIR}/README.md)
  ##

else()

  # We do not have python3, so we use existing amalgamated files instead of generating them ...
  # (Do not do this if the source and destination are the same!)
  if (NOT (${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_BINARY_DIR}))
    add_custom_command(
      OUTPUT ${SINGLEHEADER_FILES}
      COMMAND ${CMAKE_COMMAND} -E copy
        ${SINGLEHEADER_REPO_FILES}
        ${CMAKE_CURRENT_BINARY_DIR}
      DEPENDS ${SINGLEHEADER_REPO_FILES}
    )
  endif()

endif()

add_custom_target(singleheader_tests)
add_dependencies(all_tests singleheader_tests)

#
# Do not depend on singleheader files directly: depend on this target instead.
# Otherwise the custom command may get triggered multiple times and race with itself!
#
add_custom_target(singleheader-files DEPENDS ${SINGLEHEADER_FILES})

#
# Include this if you intend to #include "simdjson2.cpp" in your own .cpp files.
#
add_library(simdjson2-singleheader-include-source INTERFACE)
target_include_directories(simdjson2-singleheader-include-source INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>)
add_dependencies(simdjson2-singleheader-include-source singleheader-files)

#
# Include this to get "simdjson2.cpp" included in your project as one of the sources.
#
add_library(simdjson2-singleheader-source INTERFACE)
target_sources(simdjson2-singleheader-source INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/simdjson2.cpp>)
target_link_libraries(simdjson2-singleheader-source INTERFACE simdjson2-singleheader-include-source)

#
# We do not want CMake to update the original source files automatically "in passing" as
# part of a test that generates the files anew. This kind of side-effect is  bad.
#
# add_dependencies(simdjson2-singleheader-source amalgamate) <=== NO NO NO
#
#

include(${PROJECT_SOURCE_DIR}/cmake/add_compile_only_test.cmake)


#
# Test the generated simdjson2.cpp/simdjson2.h using the generated amalgamate_demo.cpp
#
# Under Windows you should not mix static and dynamic. Pick one. The following test is static.
if(NOT SIMDJSON2_LEGACY_VISUAL_STUDIO AND NOT SIMDJSON2_WINDOWS_DLL)
  add_executable(amalgamate_demo $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/amalgamate_demo.cpp>)
  add_library(amalgamate_demo_simdjson2 $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/simdjson2.cpp>)
  target_link_libraries(amalgamate_demo amalgamate_demo_simdjson2 simdjson2-internal-flags)
  target_include_directories(amalgamate_demo_simdjson2 PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>)
  target_include_directories(amalgamate_demo PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>)
  add_test(amalgamate_demo amalgamate_demo ${EXAMPLE_JSON} ${EXAMPLE_NDJSON})
  set_property(TEST amalgamate_demo APPEND PROPERTY LABELS per_implementation singleheader)
  add_dependencies(singleheader_tests amalgamate_demo)
  MESSAGE( STATUS "Including amalgamate_demo test. ${SIMDJSON2_WINDOWS_DLL}" )
else()
  MESSAGE( STATUS "You either have an old Visual Studio or you are building a DLL, amalgamate_demo test disabled." )
endif()

# Under Windows you should not mix static and dynamic. Pick one. The following test is static.
if(NOT SIMDJSON2_LEGACY_VISUAL_STUDIO AND NOT SIMDJSON2_WINDOWS_DLL)
  add_library(simdjson2-singleheader STATIC "")
  target_link_libraries(simdjson2-singleheader simdjson2-singleheader-source simdjson2-internal-flags)
  add_compile_only_test(simdjson2-singleheader)
  set_property(TEST simdjson2-singleheader APPEND PROPERTY LABELS per_implementation singleheader)
  add_dependencies(singleheader_tests simdjson2-singleheader)
  MESSAGE( STATUS "Including simdjson2-singleheader test." )
else()
  MESSAGE( STATUS "You either have an old Visual Studio or you are building a DLL, simdjson2-singleheader test disabled." )
endif()

#
# Test the existing simdjson2.cpp/simdjson2.h using the existing amalgamate_demo.cpp, using
# the files from the repository.
#
# By design, this will fail if the original files are missing (it should).
#
if (NOT (${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_BINARY_DIR}))

  add_library(simdjson2-singleheader-include-source-direct-from-repository INTERFACE)
  target_include_directories(simdjson2-singleheader-include-source-direct-from-repository INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

  add_library(simdjson2-singleheader-source-direct-from-repository INTERFACE)
  target_sources(simdjson2-singleheader-source-direct-from-repository INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
  target_link_libraries(simdjson2-singleheader-source-direct-from-repository INTERFACE simdjson2-singleheader-include-source-direct-from-repository)

  # If you want to use simdjson2 as a DLL under Windows, it makes little sense to run the following static test. Furthermore
  # when a DLL was built, we pass SIMDJSON2_USING_WINDOWS_DYNAMIC_LIBRARY=1 within the simdjson2-flags so that static compilation
  # breaks. Simply put: under Windows you should not mix static and dynamic. Pick one.
  if(NOT SIMDJSON2_LEGACY_VISUAL_STUDIO AND NOT SIMDJSON2_WINDOWS_DLL)
    add_executable(amalgamate_demo_direct_from_repository ${CMAKE_CURRENT_SOURCE_DIR}/amalgamate_demo.cpp)
    add_library(amalgamate_demo_direct_from_repository_simdjson2 ${CMAKE_CURRENT_SOURCE_DIR}/simdjson2.cpp)
    target_link_libraries(amalgamate_demo_direct_from_repository amalgamate_demo_direct_from_repository_simdjson2 simdjson2-singleheader-include-source-direct-from-repository simdjson2-internal-flags)
    add_test(amalgamate_demo_direct_from_repository amalgamate_demo_direct_from_repository ${EXAMPLE_JSON} ${EXAMPLE_NDJSON})
    set_property(TEST amalgamate_demo_direct_from_repository APPEND PROPERTY LABELS per_implementation singleheader)
    add_dependencies(singleheader_tests amalgamate_demo_direct_from_repository)
    MESSAGE( STATUS "Including amalgamate_demo_direct_from_repository test." )
  else()
    MESSAGE( STATUS "You either have an old Visual Studio or you are building a DLL, amalgamate_demo test disabled." )
  endif()
endif()