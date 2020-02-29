
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
  rendermodel.cpp
  resource.cpp
  ship.cpp
  test.cpp
  universe.cpp
  utility.cpp
  vector.cpp
)

SET(libamethyst_CL
  histdist.cl
  rk4_copy3d.cl
  rk4_copyrot.cl
  rk4_finalsum.cl
  rk4_grav.cl
  rk4_reductionscale.cl
  rk4_scale.cl
  rk4_scalesum.cl
  rk4_sum.cl
)

SET(amethystcmd_SRCS
  main.cpp
)

INCLUDE_DIRECTORIES(
  ${OPENCL_INCLUDE_DIR}
  ${Boost_INCLUDE_DIR}
  ${HDF5_INCLUDE_DIR}
  ${Amethyst_SOURCE_DIR}/support
  ${YAML_CPP_SOURCE_DIR}/include
  ${Amethyst_SOURCE_DIR}
  ${Amethyst_BINARY_DIR}
)

link_directories (
  ${Boost_LIBRARY_DIRS}
)

ADD_RESOURCES(libamethyst_cl ${libamethyst_CL})

ADD_LIBRARY(libamethyst
  ${libamethyst_SRCS}
  ${libamethyst_cl}
)



ADD_EXECUTABLE(amethystcmd ${amethystcmd_SRCS})

TARGET_LINK_LIBRARIES(
  libamethyst
  libsha2
  yaml-cpp
  ${Boost_TIMER_LIBRARY}
  ${Boost_SYSTEM_LIBRARY}
  ${Boost_DATE_TIME_LIBRARY}
  ${Boost_FILESYSTEM_LIBRARY}
  ${Boost_IOSTREAMS_LIBRARY}
  ${OPENCL_LIBRARIES}
  ${OPENGL_LIBRARIES}
  ${HDF5_LIBRARIES}
  ${ZLIB_LIBRARY}
  ${CMAKE_DL_LIBS}
)

TARGET_LINK_LIBRARIES(
  amethystcmd
  libamethyst
)

install(TARGETS libamethyst DESTINATION lib)
install(TARGETS amethystcmd DESTINATION bin)

### Testing 

file(GLOB TEST_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} test/*.cpp)

#Run through each source
foreach(testSrc ${TEST_SRCS})
        #Extract the filename without an extension (NAME_WE)
        get_filename_component(testName ${testSrc} NAME_WE)

        #Add compile target
        add_executable(${testName} ${testSrc})

        #link to Boost libraries AND your targets and dependencies
        target_link_libraries(${testName} ${Boost_LIBRARIES} libamethyst)

        #I like to move testing binaries into a testBin directory
        set_target_properties(${testName} PROPERTIES 
            RUNTIME_OUTPUT_DIRECTORY  ${CMAKE_CURRENT_BINARY_DIR}/test)

        #Finally add it to test execution - 
        #Notice the WORKING_DIRECTORY and COMMAND
        add_test(NAME ${testName} 
                 WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/test
                 COMMAND ${CMAKE_CURRENT_BINARY_DIR}/test/${testName} ${CMAKE_CURRENT_SOURCE_DIR}/test )
endforeach(testSrc)
