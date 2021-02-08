
add_library(sha2 STATIC
  lib/sha2_ctx.cpp
)
add_library(sha2::sha2 ALIAS sha2)
set_property(TARGET sha2 PROPERTY POSITION_INDEPENDENT_CODE ON)

target_include_directories(sha2
  PUBLIC
    ../../
)

target_link_libraries(sha2
)
