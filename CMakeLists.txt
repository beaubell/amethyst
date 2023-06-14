cmake_minimum_required(VERSION 3.16...3.19)

project(Amethyst LANGUAGES C CXX VERSION 0.1.0)

enable_testing()

set(CMAKE_CXX_STANDARD 20)
option(AMETHYST_GUI "Enable OpenGL Frontend" ON)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMake.local)
include(CMakeRC)

#set(Boost_USE_STATIC_LIBS ON)

find_package(SDL2)
find_package(Boost 1.71.0 COMPONENTS chrono date_time filesystem system thread timer iostreams unit_test_framework REQUIRED)
find_package(OpenGL)
find_package(GLEW)
find_package(OpenCL)
find_package(OpenAL)
find_package(Vulkan)
find_package(Freetype)
find_package(HDF5 COMPONENTS C CXX REQUIRED)
find_package(ZLIB)
find_package(Threads)

include(CheckIncludeFile)
check_include_file(execinfo.h HAVE_EXECINFO_H)

include(TestBigEndian)
TEST_BIG_ENDIAN(WORDS_BIGENDIAN)

include(CheckTypeSize)
CHECK_TYPE_SIZE("short"       SIZEOF_SHORT)
CHECK_TYPE_SIZE("int"         SIZEOF_INT)
CHECK_TYPE_SIZE("float"       SIZEOF_FLOAT)
CHECK_TYPE_SIZE("double"      SIZEOF_DOUBLE)
CHECK_TYPE_SIZE("long double" SIZEOF_LONG_DOUBLE)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h)

# Workaround for de-deprecated volitile handling in glm.  Remove when C++23 is properly supported.
if ( CMAKE_COMPILER_IS_GNUCC )
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wno-volatile")
endif()
if ( MSVC )
#set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS}")
endif()

add_subdirectory(support)
add_subdirectory(lib)
add_subdirectory(server)

# Build only the server if no gui components exist
if( SDL2_FOUND AND AMETHYST_GUI )
    add_subdirectory(support/FTGL)
    add_subdirectory(amethyst-gl)
endif()

include(CPack)
