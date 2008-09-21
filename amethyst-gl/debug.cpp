/***************************************************************************
 *  Debugging helping functions                                            *
 *  (c) 2006 Beau V.C. Bellamy (beau@stellarnetservices.net)               *
 ***************************************************************************/

#include "debug.h"

#include <iostream>
#ifdef HAVE_EXECINFO_H
  #include <execinfo.h>
#endif

#define MAX_BT 30

void print_vector(const char *title, const Cartesian_Vector &vector) {

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

void print_object(Object &obj)
{
    print_vector("Location ", obj.location);
    print_vector("Velocity ", obj.velocity);
    print_vector("Accelera ", obj.acceleration);
    printf("Attitude, W: %f, X: %f, Y: %f, Z: %f\n", obj.attitude.w, obj.attitude.x, obj.attitude.y, obj.attitude.z);
}


void print_trace(void)
{
#ifdef HAVE_EXECINFO_H

    void *array[MAX_BT];
    size_t size;
    char **strings;
    size_t i;

    size = backtrace (array, MAX_BT);
    strings = backtrace_symbols (array, size);

    printf ("Obtained %zd stack frames.\n", size);

    for (i = 0; i < size; i++)
        printf ("%s\n", strings[i]);

    free (strings);
#endif
}
