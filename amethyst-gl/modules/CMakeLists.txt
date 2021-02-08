
add_library(uiw_fps MODULE
    mod_uiw_fps.cpp
)

target_include_directories(uiw_fps
  PRIVATE
    $<TARGET_PROPERTY:amethyst-glcore,INCLUDE_DIRECTORIES>
)


########
add_library(uiw_test MODULE
    mod_uiw_test.cpp
)

target_include_directories(uiw_test
  PRIVATE
    $<TARGET_PROPERTY:amethyst-glcore,INCLUDE_DIRECTORIES>
)


########
add_library(uiw_log MODULE
    mod_uiw_log.cpp
)

target_include_directories(uiw_log
  PRIVATE
    $<TARGET_PROPERTY:amethyst-glcore,INCLUDE_DIRECTORIES>
)


########
add_library(uiw_shipstats MODULE
    mod_uiw_shipstats.cpp
)

target_include_directories(uiw_shipstats
  PRIVATE
    $<TARGET_PROPERTY:amethyst-glcore,INCLUDE_DIRECTORIES>
)


install(TARGETS uiw_fps uiw_test uiw_log uiw_shipstats
    DESTINATION lib
)
