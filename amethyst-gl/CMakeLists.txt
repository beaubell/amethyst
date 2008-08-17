#PROJECT(libamethyst C)

SET(amethystgl_SRCS
  config_xml.cpp
  debug.cpp
  global.cpp
  hud.cpp
  input.cpp
  joystick.cpp
  model.cpp
  net.cpp
  opengl.cpp
  scene.cpp
  stars.cpp
  texture.cpp
  thread.cpp
  timer.cpp
  amethyst-gl.cpp
)

INCLUDE_DIRECTORIES(
  ../
  ${FREETYPE_INCLUDE_DIR}
  ../support/FTGL/include
  ${LIBXML2_INCLUDE_DIR}
  ${SDL_INCLUDE_DIR}
  ${SDLNET_INCLUDE_DIRS}
)


link_directories (${SDL_LIBRARY})  
ADD_EXECUTABLE(amethystgl ${amethystgl_SRCS})
TARGET_LINK_LIBRARIES(amethystgl freetype SDL SDL_net xml2 GL GLU libamethyst libftgl)


#ENABLE_TESTING()
#ADD_TEST(SimpleTest ${EXECUTABLE_OUTPUT_PATH}/SimpleTest Hello)

