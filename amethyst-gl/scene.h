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

using lib::Object;

void scene_render(void);

void scene_add_object(Object *);
void scene_select_object_next();
void scene_target_object_next();

extern std::list<Object *>  object_list;

} // namespace client
} // namespace amethyst

#endif // AMETHYST_CLIENT_SCENE_H
