#ifndef AMETHYST_CLIENT_STARS_H
#define AMETHYST_CLIENT_STARS_H

/***********************************************************************
 Amethyst-GL
  - Star generation function declarations

 Authors (c):
 2006-2008 Beau V.C. Bellamy (beau@stellarnetservices.net)

 $Revision$
 $LastChangedDate$
 $LastChangedBy$
 ***********************************************************************/


#include <string>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace amethyst {
namespace client {

void stars_load(std::string &file);
void stars_free(void);
void stars_render(const glm::mat4 &projMatrix, const glm::mat4 &viewMatrix);

} // namespace client
} // namespace amethyst

#endif /* AMETHYST_CLIENT_STARS_H */
