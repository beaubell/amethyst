

add_library(ftgl STATIC
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
  ./src/FTGLTextHandle.cpp
)
add_library(ftgl::ftgl ALIAS ftgl)
set_property(TARGET ftgl PROPERTY POSITION_INDEPENDENT_CODE ON)

target_include_directories(ftgl
  PUBLIC
    include
)

target_compile_definitions(ftgl
  PRIVATE
    FTGL_LIBRARY_STATIC=TRUE
)

target_link_libraries(ftgl
  PUBLIC
    Freetype::Freetype
    OpenGL::GL
    OpenGL::GLU
    glm::glm
)

