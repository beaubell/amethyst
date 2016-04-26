#ifndef AMETHYST_CLIENT_SCENE_H
#define AMETHYST_CLIENT_SCENE_H

/***********************************************************************
 Amethyst-GL
  - Scene randering function prototypes

 Authors (c):
 2006-2008 Beau V.C. Bellamy (beau@stellarnetservices.net)

 $Revision$
 $LastChangedDate$
 $LastChangedBy$
 ***********************************************************************/

#include <list>
#include "lib/object.h"

namespace amethyst {
namespace client {

enum class Eye {MONO = 0, LEFT = 1, RIGHT = 2};

void scene_render(const Eye eye = Eye::MONO);

void scene_add_object(lib::Object::sptr);
void scene_select_object_next();
void scene_target_object_next();
void scene_control_ship_next();

extern std::list<lib::Object::sptr>  object_list;

glm::dmat4 get_proj(const Eye eye = Eye::MONO);
glm::dmat4 set_camera(const lib::Quaternion &attitude, const double distance, const Eye eye = Eye::MONO);

} // namespace client
} // namespace amethyst

#endif // AMETHYST_CLIENT_SCENE_H
