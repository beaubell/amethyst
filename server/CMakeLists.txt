

SET(CMAKE_CXX_FLAGS ${DEBUG_CXX_FLAGS})

SET(amethystserver_SRCS
  main.cpp
  connection.cpp
  connection_manager.cpp
  server.cpp
)

INCLUDE_DIRECTORIES(
  ../
  ${Boost_INCLUDE_DIR}
  ${Amethyst_SOURCE_DIR}/include
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
  ${Boost_FILESYSTEM_LIBRARY}
  ${CMAKE_THREAD_LIBS_INIT}
)

INSTALL(TARGETS amethyst-server DESTINATION bin)
