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

ADD_LIBRARY(libamethyst ${libamethyst_SRCS})

#ENABLE_TESTING()
#ADD_TEST(SimpleTest ${EXECUTABLE_OUTPUT_PATH}/SimpleTest Hello)

