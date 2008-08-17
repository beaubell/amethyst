PROJECT(Amethyst)

INCLUDE(./cmake/FindFreetype.cmake)

FIND_PACKAGE(LibXml2)
FIND_PACKAGE(SDL)
FIND_PACKAGE(SDL_net)

SUBDIRS(support/FTGL lib amethyst-gl server)
INCLUDE_DIRECTORIES(${FREETYPE_INCLUDE_DIR} lib)

