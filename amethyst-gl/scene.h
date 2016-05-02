#ifndef AMETHYST_CLIENT_SCENE_H
#define AMETHYST_CLIENT_SCENE_H

/***********************************************************************
 Amethyst-GL
  - Scene randering function prototypes

 Authors (c):
 2006-2016 Beau V.C. Bellamy (bellamy.beau@gmail.com)
 ***********************************************************************/

#include "camera.h"
#include "lib/object.h"

#include <list>


namespace amethyst {
namespace client {

void scene_render(const Eye eye = Eye::MONO);

void scene_add_object(lib::Object::sptr);
void scene_select_object_next();
void scene_target_object_next();
void scene_control_ship_next();

extern std::list<lib::Object::sptr>  object_list;

} // namespace client
} // namespace amethyst

#endif // AMETHYST_CLIENT_SCENE_H
