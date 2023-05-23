
cmrc_add_resource_library(libamethyst_cl ALIAS amrc::cl NAMESPACE cl
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

add_library(libamethyst
  cl.cpp
  console.cpp
  console_io.cpp
  console_menu.cpp
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

set_property(TARGET libamethyst PROPERTY POSITION_INDEPENDENT_CODE ON)

target_link_libraries(libamethyst
  PUBLIC
    Vulkan::Vulkan
    OpenCL::OpenCL
    glm::glm
    Threads::Threads
    Microsoft.GSL::GSL
)

target_include_directories(libamethyst
  PUBLIC
    ..
    ${HDF5_CXX_INCLUDE_DIRS}
)

target_link_libraries(libamethyst
  PUBLIC
    amrc::cl
    sha2::sha2
    yaml-cpp
    Boost::chrono
    Boost::timer
    Boost::system
    Boost::date_time
    Boost::filesystem
    Boost::iostreams
    Vulkan::Vulkan
    OpenCL::OpenCL
    OpenGL::GLX
    ${HDF5_LIBRARIES}
    ${ZLIB_LIBRARY}
)

add_executable(amethystcmd
  main.cpp
)

target_link_libraries(amethystcmd
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

        target_compile_definitions(${testName}
	    PRIVATE
	      ${Boost_LIB_DIAGNOSTIC_DEFINITIONS}
	)

        #link to Boost libraries AND your targets and dependencies
        target_link_libraries(${testName}
            Boost::unit_test_framework
            libamethyst
        )

        #I like to move testing binaries into a testBin directory
        set_target_properties(${testName} PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY  ${CMAKE_CURRENT_BINARY_DIR}/test)

        #Finally add it to test execution -
        #Notice the WORKING_DIRECTORY and COMMAND
        add_test(NAME ${testName}
                 WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/test
                 COMMAND ${CMAKE_CURRENT_BINARY_DIR}/test/${testName} -- ${CMAKE_CURRENT_SOURCE_DIR}/test )
endforeach(testSrc)
