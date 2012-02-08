SET(libFTGL_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include CACHE PATH "Location of FTGL includes")

SET(CMAKE_CXX_FLAGS ${SUPPORT_CXX_FLAGS})

INCLUDE_DIRECTORIES(include ${FREETYPE_INCLUDE_DIR})

SET(libFTGL_SRCS
  ./src/FTBitmapGlyph.cpp
  ./src/FTFace.cpp
  ./src/FTGLOutlineFont.cpp
  ./src/FTGlyph.cpp
  ./src/FTPixmapGlyph.cpp
  ./src/FTTextureGlyph.cpp
  ./src/FTCharmap.cpp
  ./src/FTFont.cpp
  ./src/FTGLPixmapFont.cpp
  ./src/FTGlyphContainer.cpp
  ./src/FTPoint.cpp
  ./src/FTVectoriser.cpp
  ./src/FTContour.cpp
  ./src/FTGLBitmapFont.cpp
  ./src/FTGLPolygonFont.cpp
  ./src/FTLibrary.cpp
  ./src/FTPolyGlyph.cpp
  ./src/FTExtrdGlyph.cpp
  ./src/FTGLExtrdFont.cpp
  ./src/FTGLTextureFont.cpp 
  ./src/FTOutlineGlyph.cpp 
  ./src/FTSize.cpp
)

ADD_LIBRARY(libftgl SHARED ${libFTGL_SRCS})
TARGET_LINK_LIBRARIES(libftgl ${FREETYPE_LIBRARY} ${OPENGL_gl_LIBRARY} ${OPENGL_glu_LIBRARY})
INSTALL(TARGETS libftgl DESTINATION lib)

