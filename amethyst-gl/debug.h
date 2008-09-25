#ifndef DEBUG_H
#define DEBUG_H

/***********************************************************************
 Amethyst-GL
  - Debugging helper functions

 Authors (c):
 2006-2008 Beau V.C. Bellamy (beau@stellarnetservices.net)

 $Revision: 150 $
 $LastChangedDate: 2008-09-23 13:43:06 -0800 (Tue, 23 Sep 2008) $
 $LastChangedBy: beau $
 ***********************************************************************/

#include "lib/vector.h"
#include "lib/object.h"

using namespace amethyst;

#if 0
void print_vector(char *title, const Cartesian_Vector &vector);
void print_camera(Cartesian_Vector &real_pos,
                  Cartesian_Vector &real_view,
                  Cartesian_Vector &real_up);
#endif

void print_object(amethyst::Object &obj);

void print_trace(void);

#endif /* GLOBAL_H */

