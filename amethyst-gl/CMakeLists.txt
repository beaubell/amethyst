
SET(CMAKE_CXX_FLAGS ${DEBUG_CXX_FLAGS})

SET(amethystgl_SRCS
  science/uiw_simstats.cpp
  science/gravpotential.cpp
  config_xml.cpp
  debug.cpp
  global.cpp
  hud.cpp
  input.cpp
  joystick.cpp
  log.cpp
  model.cpp
  model_xml.cpp
  module.cpp
  net.cpp
  opengl.cpp
  opengl_shader.cpp
  scene.cpp
  scene_object.cpp
  scene_xml.cpp
  sound.cpp
  stars.cpp
  texture.cpp
  thread.cpp
  timer.cpp
  ui.cpp
  amethyst-gl.cpp
  bootstrap.cpp
  ${amthyst-uiwindows}
)

INCLUDE_DIRECTORIES(
  ${OPENCL_INCLUDE_DIR}
  ${FREETYPE_INCLUDE_DIR}
  ${libFTGL_INCLUDE_DIR}
  ${LIBXML2_INCLUDE_DIR}
  ${SDL_INCLUDE_DIR}
  ${Boost_INCLUDE_DIR}
  ${Amethyst_SOURCE_DIR}/support
  ${Amethyst_SOURCE_DIR}
  ${Amethyst_BINARY_DIR}

)


link_directories (${SDL_LIBRARY_DIR} ${Boost_LIBRARY_DIRS})  
ADD_EXECUTABLE(amethyst-gl ${amethystgl_SRCS})
TARGET_LINK_LIBRARIES(amethyst-gl
  ${SDL_LIBRARY}
  ${LIBXML2_LIBRARIES}
  ${FREETYPE_LIBRARY}
  ${OPENGL_gl_LIBRARY}
  -lGLEW
  ${CMAKE_DL_LIBS}
  libamethyst
  libftgl
  ${Boost_TIMER_LIBRARY}
  ${Boost_THREAD_LIBRARY})
INSTALL(TARGETS amethyst-gl DESTINATION bin)

SUBDIRS(modules science)
