#PROJECT(libamethyst C)

SET(CMAKE_CXX_FLAGS ${DEBUG_CXX_FLAGS})

SET(moduiwfps_SRCS
  mod_uiw_fps.cpp
)

SET(mod_uiw_test_SRCS
  mod_uiw_test.cpp
)

INCLUDE_DIRECTORIES(
  ${amethystgl_SOURCE_DIR}
  ${Amethyst_SOURCE_DIR}
  ${Boost_INCLUDE_DIR}
)

ADD_LIBRARY(uiwfps MODULE ${moduiwfps_SRCS})
ADD_LIBRARY(uiw_test MODULE ${mod_uiw_test_SRCS})

#TARGET_LINK_LIBRARIES(
#  uiwfps
#  libamethyst
#  ${Boost_SYSTEM_LIBRARY}
#  ${Boost_DATE_TIME_LIBRARY}
#  ${Boost_FILESYSTEM_LIBRARY}
#)

