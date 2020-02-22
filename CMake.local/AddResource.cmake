
FUNCTION(ADD_RESOURCES out_var)
  SET(result)
  FOREACH(in_f ${ARGN})
    FILE(RELATIVE_PATH src_f ${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/${in_f})
    SET(out_f "${CMAKE_CURRENT_BINARY_DIR}/${in_f}.o")
    ADD_CUSTOM_COMMAND(OUTPUT ${out_f}
      COMMAND ${CMAKE_LINKER} -r -b binary -o ${out_f} ${src_f}
      DEPENDS ${in_f}
      WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
      COMMENT "Including file ${out_f}"
      VERBATIM
      )
    LIST(APPEND result ${out_f})
  ENDFOREACH()
  SET(${out_var} "${result}" PARENT_SCOPE)
ENDFUNCTION()
