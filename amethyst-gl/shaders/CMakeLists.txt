
cmrc_add_resource_library(shader_SRCS ALIAS amrc::shader_srcs
  baseline.vert
  baseline.frag
  fb.vert
  fb.frag
  hud.vert
  hud.frag
  model.vert
  model.frag
  ui.vert
  ui.frag
  uifont.vert
  uifont.frag
)

set_property(TARGET shader_SRCS PROPERTY
    POSITION_INDEPENDENT_CODE ON
)
