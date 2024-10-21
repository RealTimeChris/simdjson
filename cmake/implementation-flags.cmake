#
# Implementation selection
#
set(SIMDJSON2_ALL_IMPLEMENTATIONS fallback westmere haswell icelake arm64 ppc64)

set(
    SIMDJSON2_IMPLEMENTATION ""
    CACHE STRING "\
Semicolon-separated list of implementations to include \
(${SIMDJSON2_ALL_IMPLEMENTATIONS}). If this is not set, any implementations \
that are supported at compile time and may be selected at runtime will be \
included."
)
set(
    SIMDJSON2_EXCLUDE_IMPLEMENTATION ""
    CACHE STRING "\
Semicolon-separated list of implementations to exclude \
(icelake/haswell/westmere/arm64/ppc64/fallback). By default, excludes any \
implementations that are unsupported at compile time or cannot be selected at \
runtime."
)

foreach(var IN ITEMS IMPLEMENTATION EXCLUDE_IMPLEMENTATION)
  set(var "SIMDJSON2_${var}")
  foreach(impl IN LISTS "${var}")
    if(NOT impl IN_LIST SIMDJSON2_ALL_IMPLEMENTATIONS)
      message(ERROR "\
Implementation ${impl} found in ${var} not supported by simdjson2. \
Possible implementations: ${SIMDJSON2_ALL_IMPLEMENTATIONS}")
    endif()
  endforeach()
endforeach()

macro(flag_action action var val)
  message(STATUS "${action} implementation ${impl} due to ${var}=${${var}}")
  simdjson2_add_props(
      target_compile_definitions PUBLIC
      "SIMDJSON2_IMPLEMENTATION_${impl_upper}=${val}"
  )
endmacro()

foreach(impl IN LISTS SIMDJSON2_ALL_IMPLEMENTATIONS)
  string(TOUPPER "${impl}" impl_upper)
  if(impl IN_LIST SIMDJSON2_EXCLUDE_IMPLEMENTATION)
    flag_action(Excluding SIMDJSON2_EXCLUDE_IMPLEMENTATION 0)
  elseif(impl IN_LIST SIMDJSON2_IMPLEMENTATION)
    flag_action(Including SIMDJSON2_IMPLEMENTATION 1)
  elseif(SIMDJSON2_IMPLEMENTATION)
    flag_action(Excluding SIMDJSON2_IMPLEMENTATION 0)
  endif()
endforeach()

# TODO make it so this generates the necessary compiler flags to select the
# given impl as the builtin automatically!
set(
    SIMDJSON2_BUILTIN_IMPLEMENTATION ""
    CACHE STRING "\
Select the implementation that will be used for user code. Defaults to the \
most universal implementation in SIMDJSON2_IMPLEMENTATION (in the order \
${SIMDJSON2_ALL_IMPLEMENTATIONS}) if specified; otherwise, by default the \
compiler will pick the best implementation that can always be selected given \
the compiler flags."
)
if(NOT SIMDJSON2_BUILTIN_IMPLEMENTATION STREQUAL "")
  simdjson2_add_props(
      target_compile_definitions PUBLIC
      "SIMDJSON2_BUILTIN_IMPLEMENTATION=${SIMDJSON2_BUILTIN_IMPLEMENTATION}"
  )
else()
  # Pick the most universal implementation out of the selected implementations
  # (if any)
  foreach(impl IN LISTS SIMDJSON2_ALL_IMPLEMENTATIONS)
    if(
        impl IN_LIST SIMDJSON2_IMPLEMENTATION
        AND NOT impl IN_LIST SIMDJSON2_EXCLUDE_IMPLEMENTATION
    )
      message(STATUS "\
Selected implementation ${impl} as builtin implementation based on \
${SIMDJSON2_IMPLEMENTATION}")
      simdjson2_add_props(
          target_compile_definitions PUBLIC
          "SIMDJSON2_BUILTIN_IMPLEMENTATION=${impl}"
      )
      break()
    endif()
  endforeach()
endif()

foreach(impl IN LISTS SIMDJSON2_ALL_IMPLEMENTATIONS)
  string(TOUPPER "${impl}" impl_upper)
  option(
      "SIMDJSON2_IMPLEMENTATION_${impl_upper}"
      "Include the ${impl} implementation"
      ON
  )
  mark_as_advanced("SIMDJSON2_IMPLEMENTATION_${impl_upper}")
  if(NOT "${SIMDJSON2_IMPLEMENTATION_${impl_upper}}")
    message(DEPRECATION "\
SIMDJSON2_IMPLEMENTATION_${impl_upper} is deprecated. \
Use SIMDJSON2_IMPLEMENTATION=-${impl} instead")
     simdjson2_add_props(
        target_compile_definitions PUBLIC
        "SIMDJSON2_IMPLEMENTATION_${impl_upper}=0"
    )
  endif()
endforeach()
