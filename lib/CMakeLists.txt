#PROJECT(libamethyst C)

SET(libamethyst_SRCS
  console.cpp
  console_history.cpp
  console_io.cpp
  console_menu.cpp
  console_menu_main.cpp
  engine.cpp
  main.cpp
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
)

ADD_LIBRARY(libamethyst ${libamethyst_SRCS})
