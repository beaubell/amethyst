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
#include "scene_object.h"

namespace amethyst {
namespace client {


void scene_render(void);

void scene_add_object(Scene_Object_Base *);
void scene_select_object_next();
void scene_target_object_next();
void scene_control_ship_next();

extern std::list<Scene_Object_Base *>  object_list;

} // namespace client
} // namespace amethyst

#endif // AMETHYST_CLIENT_SCENE_H
