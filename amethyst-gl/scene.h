#ifndef SCENE_H
#define SCENE_H

/***************************************************************************
 *   Scene Rendering Functions                                             *
 *  (c) 2006 Beau V.C. Bellamy (beau@stellarnetservices.net)               *
 ***************************************************************************/

#include <list>
#include "lib/object.h"

void RenderScene(void);

void scene_add_object(amethyst::Object *);

extern std::list<amethyst::Object *>  object_list;

#endif // SCENE_H
