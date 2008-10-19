
SET(CMAKE_CXX_FLAGS ${DEBUG_CXX_FLAGS})

SET(amethystgl_SRCS
  config_xml.cpp
  debug.cpp
  global.cpp
  hud.cpp
  input.cpp
  joystick.cpp
  model.cpp
  model_xml.cpp
  net.cpp
  opengl.cpp
  opengl_shader.cpp
  scene.cpp
  scene_xml.cpp
  stars.cpp
  texture.cpp
  thread.cpp
  timer.cpp
  ui.cpp
  amethyst-gl.cpp
)

INCLUDE_DIRECTORIES(
  ../
  ${FREETYPE_INCLUDE_DIR}
  ../support/FTGL/include
  ${LIBXML2_INCLUDE_DIR}
  ${SDL_INCLUDE_DIR}
  ${SDLNET_INCLUDE_DIRS}
  ${Boost_INCLUDE_DIR}
  ../support/
)


link_directories (${SDL_LIBRARY} ${Boost_LIBRARY_DIRS})  
ADD_EXECUTABLE(amethyst-gl ${amethystgl_SRCS})
TARGET_LINK_LIBRARIES(amethyst-gl freetype SDL SDL_net xml2 GL GLU libamethyst libftgl ${Boost_THREAD_LIBRARY})

