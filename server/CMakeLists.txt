
add_executable(amethyst-server
  main.cpp
  connection.cpp
  connection_manager.cpp
  server.cpp
)

target_link_libraries(amethyst-server
  libamethyst
  sha2::sha2
  Boost::system
  Boost::filesystem
  Boost::date_time
  Threads::Threads
)

INSTALL(TARGETS amethyst-server DESTINATION bin)
