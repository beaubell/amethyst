CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(Amethyst)

SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMake.local)

FIND_PACKAGE(LibXml2)
FIND_PACKAGE(SDL)
FIND_PACKAGE(Boost 1.35.0 COMPONENTS date_time filesystem system thread)
FIND_PACKAGE(OpenGL)
FIND_PACKAGE(GLUT)
FIND_PACKAGE(OpenCL)
FIND_PACKAGE(OpenAL)
FIND_PACKAGE(FreeType)
#FIND_PACKAGE(Subversion)
#IF(Subversion_FOUND)
#    Subversion_WC_INFO(${PROJECT_SOURCE_DIR} AMETHYST)
#    MESSAGE("Current revision is ${AMETHYST_WC_REVISION}")
#ENDIF(Subversion_FOUND)
INCLUDE(CheckIncludeFile)
CHECK_INCLUDE_FILE(execinfo.h HAVE_EXECINFO_H)

INCLUDE(TestBigEndian)
TEST_BIG_ENDIAN(WORDS_BIGENDIAN)

INCLUDE (CheckTypeSize)
CHECK_TYPE_SIZE("short"       SIZEOF_SHORT)
CHECK_TYPE_SIZE("int"         SIZEOF_INT)
CHECK_TYPE_SIZE("float"       SIZEOF_FLOAT)
CHECK_TYPE_SIZE("double"      SIZEOF_DOUBLE)
CHECK_TYPE_SIZE("long double" SIZEOF_LONG_DOUBLE)

CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h)

#SET( DEBUG_CXX_FLAGS "-D HAVE_CONFIG_H -ggdb -Wall -Wextra -Wconversion -Wold-style-cast")
SET( DEBUG_CXX_FLAGS "-D HAVE_CONFIG_H -ggdb -Wall -Wextra ")
#SET( DEBUG_CXX_FLAGS "-D HAVE_CONFIG_H -O3")
SET( SUPPORT_CXX_FLAGS "-O3")

#Hardset the compiler for debugging
#SET( CMAKE_CXX_COMPILER "/usr/bin/gcc-4.4.3")

# Build only the server if no gui components exist
IF( NOT SDLMAIN_LIBRARY )
	       SUBDIRS(support/boost/crypto lib server)
	ELSE()
	       SUBDIRS(support/FTGL support/boost/crypto lib amethyst-gl server)
ENDIF()

INCLUDE_DIRECTORIES(${FREETYPE_INCLUDE_DIR_freetype2} lib)

include(CPack)
