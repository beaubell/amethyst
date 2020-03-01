#pragma once
/***********************************************************************
 Amethyst-GL
  - Heads up display function declarations

 Authors (c):
 2006-2020 Beau V.C. Bellamy (bellamy.beau@gmail.com)
 ***********************************************************************/

#include "camera.h"

namespace amethyst {
namespace client {

void hud_setup(void);
void hud_shutdown(void);
void hud_render(const Camera& camera, const Eye eye = Eye::MONO);

} // namespace client
} // namespace amethyst
