#PROJECT(libamethyst C)

SET(CMAKE_CXX_FLAGS ${DEBUG_CXX_FLAGS})

SET(libamethyst_SRCS
  console.cpp
  console_history.cpp
  console_io.cpp
  console_menu.cpp
  console_menu_main.cpp
  engine.cpp
  main.cpp
  manifest.cpp
  net.cpp
  object.cpp
  orientation.cpp
  physics.cpp
  test.cpp
  universe.cpp
  utility.cpp
  vector.cpp
)

INCLUDE_DIRECTORIES(
  ../
  ${Boost_INCLUDE_DIR}
  ../support/
)

link_directories (
  ${Boost_LIBRARY_DIRS}
)


ADD_LIBRARY(libamethyst ${libamethyst_SRCS})

TARGET_LINK_LIBRARIES(
  libamethyst
  libsha2
  ${Boost_SYSTEM_LIBRARY}
  ${Boost_DATE_TIME_LIBRARY}
  ${Boost_FILESYSTEM_LIBRARY}
)

