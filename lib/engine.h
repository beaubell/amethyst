/*
   Granularized Linear Physics Engine, Declarations
   Amethyst Physics Library (c) 2003
   Author: Beau V.C. Bellamy
*/

// 
// 

#ifndef ENGINE_H
#define ENGINE_H

#include "object.h"

#ifdef __GNUG__
#pragma interface
#endif

namespace amethyst {

  void iterate_engine (Object *head, double time);

}

#endif /* ENGINE_H */
