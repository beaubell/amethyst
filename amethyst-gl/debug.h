#ifndef AMETHYST_CLIENT_DEBUG_H
#define AMETHYST_CLIENT_DEBUG_H

/***********************************************************************
 Amethyst-GL
  - Debugging helper functions

 Authors (c):
 2006-2008 Beau V.C. Bellamy (beau@stellarnetservices.net)

 $Revision$
 $LastChangedDate$
 $LastChangedBy$
 ***********************************************************************/

#include "lib/vector.h"
#include "lib/object.h"

namespace amethyst {
namespace client {

using lib::Object;

#if 0
void print_vector(char *title, const Cartesian_Vector &vector);
void print_camera(Cartesian_Vector &real_pos,
                  Cartesian_Vector &real_view,
                  Cartesian_Vector &real_up);
#endif

void print_object(Object &obj);

void print_trace(void);

} // namespace client
} // namespace amethyst;

#endif /* AMETHYST_CLIENT_DEBUG_H */

