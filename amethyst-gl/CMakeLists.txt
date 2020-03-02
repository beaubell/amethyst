
SET(CMAKE_CXX_FLAGS ${DEBUG_CXX_FLAGS})

SUBDIRS(modules science shaders resources)

SET(amethystgl_SRCS
  science/uiw_simstats.cpp
  science/uiw_view.cpp
  science/gravpotential.cpp
  bufferobj.cpp
  camera.cpp
  debug.cpp
  framebuffer.cpp
  global.cpp
  hud.cpp
  hud_radial.cpp
  hud_orbit.cpp
  hud_objectnames.cpp
  input.cpp
  joystick.cpp
  log.cpp
  model.cpp
  model_manager.cpp
  module.cpp
  net.cpp
  opengl.cpp
  primitive.cpp
  renderbuffer.cpp
  scene.cpp
  scene_object.cpp
  shader.cpp
  shaderprog.cpp
  shaderprog_hud.cpp
  shaderprog_model.cpp
  shaderprog_ui.cpp
  shaderprog_uifont.cpp
  sound.cpp
  stars.cpp
  texture.cpp
  thread.cpp
  timer.cpp
  ui.cpp
  ui_object.cpp
  ui_widget.cpp
  ui_textbox.cpp
  amethyst-gl.cpp
  bootstrap.cpp
  ${amthyst-uiwindows}
)



INCLUDE_DIRECTORIES(
  ${OPENCL_INCLUDE_DIR}
  ${FREETYPE_INCLUDE_DIR}
  ${libFTGL_INCLUDE_DIR}
  ${SDL_INCLUDE_DIR}
  ${Boost_INCLUDE_DIR}
  ${Amethyst_SOURCE_DIR}/support
  ${YAML_CPP_SOURCE_DIR}/include
  ${Amethyst_SOURCE_DIR}
  ${Amethyst_BINARY_DIR}
)

link_directories (
  ${SDL_LIBRARY_DIR}
  ${Boost_LIBRARY_DIRS}
  ${YAML_CPP_BINARY_DIR}
)  

ADD_RESOURCES(science_CL
  science/gravpotential.cl
  science/colorizer.frag
  science/fixed.vert
)

ADD_RESOURCES(shader_SRCS
  shaders/baseline.vert
  shaders/baseline.frag
  shaders/fb.vert
  shaders/fb.frag
  shaders/hud.vert
  shaders/hud.frag
  shaders/model.vert
  shaders/model.frag
  shaders/ui.vert
  shaders/ui.frag
  shaders/uifont.vert
  shaders/uifont.frag
)

ADD_RESOURCES(resource_SRCS
  resources/config.yaml
  resources/scn_Startup.yaml
)

ADD_EXECUTABLE(amethyst-gl 
  ${amethystgl_SRCS}
  ${science_CL}
  ${shader_SRCS}
  ${resource_SRCS}
)
  
TARGET_LINK_LIBRARIES(amethyst-gl
  ${SDL_LIBRARY}
  ${FREETYPE_LIBRARY}
  ${OPENGL_gl_LIBRARY}
  -lGLEW
  ${CMAKE_DL_LIBS}
  libamethyst
  libftgl
  ${Boost_TIMER_LIBRARY}
  ${Boost_THREAD_LIBRARY}
  ${YAML_CPP_LIBRARIES}
)

INSTALL(TARGETS amethyst-gl DESTINATION bin)

