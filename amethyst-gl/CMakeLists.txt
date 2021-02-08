
add_subdirectory(science)
add_subdirectory(shaders)
add_subdirectory(resources)
add_subdirectory(modules)

add_library(amethyst-glcore SHARED
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
  ${amthyst-uiwindows}
)

target_link_libraries(amethyst-glcore
  amrc::science_cl
  amrc::shader_srcs
  amrc::res_srcs
  SDL2::SDL2
  ${CMAKE_DL_LIBS}
  libamethyst
  ftgl::ftgl
  yaml-cpp::yaml-cpp
)

set_property(TARGET amethyst-glcore PROPERTY POSITION_INDEPENDENT_CODE ON)

add_executable(amethyst-gl 
  bootstrap.cpp
)

target_link_libraries(amethyst-gl
  amethyst-glcore
)

install(TARGETS amethyst-gl DESTINATION bin)

