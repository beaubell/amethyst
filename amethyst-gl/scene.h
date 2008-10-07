#ifndef SCENE_H
#define SCENE_H

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

void scene_render(void);

void scene_add_object(amethyst::Object *);
void scene_select_object_next();
void scene_target_object_next();

extern std::list<amethyst::Object *>  object_list;

#include "lib/universe.h"

extern amethyst::Universe universe;

#endif // SCENE_H
