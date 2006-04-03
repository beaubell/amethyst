#ifndef DEBUG_H
#define DEBUG_H

/***************************************************************************
 *   Debuging helper functions protoypes                                   *
 *  (c) 2006 Beau V.C. Bellamy (beau@stellarnetservices.net)               *
 ***************************************************************************/

#include "amethyst/vector.h"

using namespace amethyst;

void print_vector(char *title, const Cartesian_Vector &vector);
void print_camera(Cartesian_Vector &real_pos,
                  Cartesian_Vector &real_view,
                  Cartesian_Vector &real_up);

#endif /* GLOBAL_H */
