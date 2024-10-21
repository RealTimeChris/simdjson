if(DEFINED SIMDJSON2_BUILD_STATIC)
  message(DEPRECATION "SIMDJSON2_BUILD_STATIC is deprecated, setting \
BUILD_SHARED_LIBS with its value and unsetting it")
  set(shared ON)
  if(SIMDJSON2_BUILD_STATIC)
    set(shared OFF)
  endif()

  set(BUILD_SHARED_LIBS "${shared}" CACHE BOOL "" FORCE)
  unset(SIMDJSON2_BUILD_STATIC CACHE)
endif()

if(DEFINED SIMDJSON2_JUST_LIBRARY)
  message(DEPRECATION "SIMDJSON2_JUST_LIBRARY is deprecated, setting \
SIMDJSON2_DEVELOPER_MODE with its value and unsetting it")
  set(dev_mode ON)
  if(SIMDJSON2_JUST_LIBRARY)
    set(dev_mode OFF)
  endif()

  set(SIMDJSON2_DEVELOPER_MODE "${dev_mode}" CACHE BOOL "" FORCE)
  unset(SIMDJSON2_JUST_LIBRARY CACHE)
endif()
