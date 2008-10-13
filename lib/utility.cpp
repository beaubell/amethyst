/*
   Utility Functions, Definitions
   Amethyst Physics Library (c) 2003
   Author: Beau V.C. Bellamy
*/

//
//

#include <stdio.h>

#include "utility.h"

#ifdef __GNUG__
#pragma implementation
#endif

namespace amethyst {

  void print_vector(const char *title, const Cartesian_Vector &vector) {

       printf("%s CV: x:%#1.16lE y:%#1.16lE z:%#1.16lE\n", title, vector.x, vector.y, vector.z);
       }


  void print_vector(const char *title, const Spherical_Vector &vector) {

       printf("%s SV: a:%#1.16lE p:%#1.16lE r:%#1.16lE\n", title, vector.a, vector.p, vector.r);
       }

  void print_vector(const char *title, const Quaternion &vector) {

       printf("%s QU: a:%#1.16lE p:%#1.16lE r:%#1.16lE\n", title, vector.x, vector.y, vector.z);
       }


    void print_object(const Object &obj)
    {

         printf("Object Summary: %s\n", obj.name.c_str());
         printf(" Mass         : %#1.16lE\n", obj.mass);
         print_vector(" Location     :", obj.location);
         print_vector(" Velocity     :", obj.velocity);
         print_vector(" Acceleration :", obj.acceleration);
         print_vector(" Force        :", obj.force);

    }

}
