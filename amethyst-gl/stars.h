#pragma once

/***********************************************************************
 Amethyst-GL
  - Starfield rendering function declarations

 Authors (c):
 2006-2020 Beau V.C. Bellamy (bellamy.beau@gmail.com)
 ***********************************************************************/

#include "lib/resource.h"

#include <string>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace amethyst {
namespace client {

void stars_load(const lib::Resource& res);
void stars_free(void);
void stars_render(const glm::mat4 &projMatrix, const glm::mat4 &viewMatrix);

} // namespace client
} // namespace amethyst
