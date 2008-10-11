

SET(amethystserver_SRCS
  main.cpp
  connection.cpp
  connection_manager.cpp
  server.cpp
)

INCLUDE_DIRECTORIES(
  ../
  ${Boost_INCLUDE_DIR}
)

link_directories (
  ${Boost_LIBRARY_DIRS}
)

ADD_EXECUTABLE(amethyst_server ${amethystserver_SRCS})

TARGET_LINK_LIBRARIES(
  amethyst_server
  libamethyst
  ${Boost_SYSTEM_LIBRARY}
)
