add_subdirectory(dom)

set(CMAKE_CXX_STANDARD 23)

include_directories( . linux )
link_libraries(simdjson-windows-headers test-data)
link_libraries(simdjson)

add_executable(benchfeatures benchfeatures.cpp)
add_executable(get_corpus_benchmark get_corpus_benchmark.cpp)

if (TARGET benchmark::benchmark)
  link_libraries(benchmark::benchmark)
  add_executable(bench_parse_call bench_parse_call.cpp)
  add_executable(bench_dom_api bench_dom_api.cpp)
  if(SIMDJSON_EXCEPTIONS)
    add_executable(bench_ondemand bench_ondemand.cpp)

    # Set the C++ standard for bench_ondemand
    target_compile_features(bench_ondemand PRIVATE cxx_std_23)
    target_compile_options(bench_ondemand PRIVATE -march=native)

    if(TARGET jsonifier)
      target_link_libraries(bench_ondemand PRIVATE jsonifier)
    endif()
    if(TARGET yyjson)
      target_link_libraries(bench_ondemand PRIVATE yyjson)
    endif()
    if(TARGET rapidjson)
      target_link_libraries(bench_ondemand PRIVATE rapidjson)
    endif()
    if(TARGET sajson)
      target_link_libraries(bench_ondemand PRIVATE sajson)
    endif()
    if(TARGET nlohmann_json)
      target_link_libraries(bench_ondemand PRIVATE nlohmann_json)
    endif()
    if(TARGET boostjson)
      target_link_libraries(bench_ondemand PRIVATE boostjson)
    endif()
  endif()
endif()


