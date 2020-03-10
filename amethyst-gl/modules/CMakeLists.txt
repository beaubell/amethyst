#PROJECT(libamethyst C)

SET(CMAKE_CXX_FLAGS ${DEBUG_CXX_FLAGS})

SET(mod_uiw_fps_SRCS
  mod_uiw_fps.cpp
)

SET(mod_uiw_test_SRCS
  mod_uiw_test.cpp
)

SET(mod_uiw_log_SRCS
  mod_uiw_log.cpp
)

SET(mod_uiw_shipstats_SRCS
  mod_uiw_shipstats.cpp
  )

INCLUDE_DIRECTORIES(
  ${amethystgl_SOURCE_DIR}
  ${Amethyst_SOURCE_DIR}
  ${Amethyst_SOURCE_DIR}/include
  ${Boost_INCLUDE_DIR}
)

ADD_LIBRARY(uiw_fps MODULE ${mod_uiw_fps_SRCS})
ADD_LIBRARY(uiw_test MODULE ${mod_uiw_test_SRCS})
ADD_LIBRARY(uiw_log MODULE ${mod_uiw_log_SRCS})
ADD_LIBRARY(uiw_shipstats MODULE ${mod_uiw_shipstats_SRCS})

INSTALL(TARGETS uiw_fps uiw_test uiw_log uiw_shipstats DESTINATION lib)
#TARGET_LINK_LIBRARIES(
#  uiwfps
#  libamethyst
#  ${Boost_SYSTEM_LIBRARY}
#  ${Boost_DATE_TIME_LIBRARY}
#  ${Boost_FILESYSTEM_LIBRARY}
#)

