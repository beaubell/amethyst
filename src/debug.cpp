/***************************************************************************
 *  Debugging helping functions                                            *
 *  (c) 2006 Beau V.C. Bellamy (beau@stellarnetservices.net)               *
 ***************************************************************************/

#include "debug.h"

#include <iostream>


void print_vector(char *title, const Cartesian_Vector &vector) {

       printf("%s CV: x:%#1.16lE y:%#1.16lE z:%#1.16lE\n", title, vector.x, vector.y, vector.z);
       }


void print_camera (Cartesian_Vector &real_pos,
                   Cartesian_Vector &real_view,
                   Cartesian_Vector &real_up)
{
        //print_vector("ship position", position);
        print_vector("real_pos     ", real_pos);
        print_vector("real_view    ", real_view);
        print_vector("real_up      ", real_up);
        //printf("RAW  SQUARE: %f \n", phys_distance(raw_view, raw_up));
        //printf("REAL SQUARE: %f \n", phys_distance(real_view, real_up));
}
