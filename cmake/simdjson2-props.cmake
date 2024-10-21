#
# Accumulate flags
#
set(simdjson2_props_script "${PROJECT_BINARY_DIR}/simdjson2-props.cmake")
set(simdjson2_props_content "")
set(simdjson2_props_flushed NO)

function(simdjson2_add_props command)
  set(args "")
  math(EXPR limit "${ARGC} - 1")
  foreach(i RANGE 1 "${limit}")
    set(value "${ARGV${i}}")
    if(value MATCHES "^(PRIVATE|PUBLIC)$")
      string(TOLOWER "${value}" value)
      set(value "\${${value}}")
    else()
      set(value "[==[${value}]==]")
    endif()
    string(APPEND args " ${value}")
  endforeach()

  set(simdjson2_props_flushed NO PARENT_SCOPE)
  set(
      simdjson2_props_content
      "${simdjson2_props_content}${command}(\"\${target}\"${args})\n"
      PARENT_SCOPE
  )
endfunction()

macro(simdjson2_flush_props)
  if(NOT simdjson2_props_flushed)
    set(simdjson2_props_flushed YES PARENT_SCOPE)
    file(WRITE "${simdjson2_props_script}" "${simdjson2_props_content}")
  endif()
endmacro()

function(simdjson2_apply_props target)
  set(private PRIVATE)
  set(public PUBLIC)
  get_target_property(TYPE "${target}" TYPE)
  if(TYPE STREQUAL "INTERFACE_LIBRARY")
    set(private INTERFACE)
    set(public INTERFACE)
  endif()

  simdjson2_flush_props()
  include("${simdjson2_props_script}")
endfunction()
