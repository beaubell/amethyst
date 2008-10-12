

SET(amethystserver_SRCS
  main.cpp
  connection.cpp
  connection_manager.cpp
  server.cpp
)

INCLUDE_DIRECTORIES(
  ../
  ${Boost_INCLUDE_DIR}
  ../support/
)

link_directories (
  ${Boost_LIBRARY_DIRS}
)

ADD_EXECUTABLE(amethyst-server ${amethystserver_SRCS})

TARGET_LINK_LIBRARIES(
  amethyst-server
  libamethyst
  libsha2
  ${Boost_SYSTEM_LIBRARY}
  ${Boost_DATE_TIME_LIBRARY}
)
