#PROJECT(libamethyst C)

SET(CMAKE_CXX_FLAGS ${DEBUG_CXX_FLAGS})

SET(libamethyst_SRCS
  cl.cpp
  console.cpp
  console_history.cpp
  console_io.cpp
  console_menu.cpp
  console_menu_main.cpp
  io_hdf5.cpp
  manifest.cpp
  matrix.cpp
  net.cpp
  object.cpp
  object_group.cpp
  orientation.cpp
  physics.cpp
  ship.cpp
  test.cpp
  universe.cpp
  utility.cpp
  vector.cpp
)

SET(amethystcmd_SRCS
  main.cpp
)

INCLUDE_DIRECTORIES(
  ${OPENCL_INCLUDE_DIR}
  ${Boost_INCLUDE_DIR}
  ${HDF5_INCLUDE_DIR}
  ${Amethyst_SOURCE_DIR}/support
  ${Amethyst_SOURCE_DIR}
  ${Amethyst_BINARY_DIR}
)

link_directories (
  ${Boost_LIBRARY_DIRS}
)


ADD_LIBRARY(libamethyst ${libamethyst_SRCS})

ADD_EXECUTABLE(amethystcmd ${amethystcmd_SRCS})

TARGET_LINK_LIBRARIES(
  libamethyst
  libsha2
  ${Boost_TIMER_LIBRARY}
  ${Boost_SYSTEM_LIBRARY}
  ${Boost_DATE_TIME_LIBRARY}
  ${Boost_FILESYSTEM_LIBRARY}
  ${OPENCL_LIBRARIES}
  ${OPENGL_LIBRARIES}
  ${HDF5_LIBRARIES_CXX}
  ${HDF5_LIBRARIES}
  ${ZLIB_LIBRARY}
)

TARGET_LINK_LIBRARIES(
  amethystcmd
  libamethyst
)

install(TARGETS libamethyst DESTINATION lib)
install(TARGETS amethystcmd DESTINATION bin)
