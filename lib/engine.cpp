/*
   Granularized Linear Physics Engine, Definitions
   Amethyst Physics Library (c) 2003
   Author: Beau V.C. Bellamy
*/

//
//

#include <stdio.h>

#include "engine.h"
#include "physics.h"
#include "utility.h"

#ifdef __GNUG__
#pragma implementation
#endif

namespace amethyst {

  void iterate_engine (Object *head, double time)
  {
       Object *obj1 = head;
       Object *obj2 = NULL;

       /* Clear Force and Acceleration Vectors */
       do
       {
            obj1->force_clear();
            //print_object(*obj1);

            obj1 = obj1->next;
       } while (obj1 != NULL);


       /* Calculate Gravitational Forces */
       obj1 = head;
       obj2 = NULL;

       do
       {
            if(obj1->next == NULL) break;

            obj2 = obj1->next;

            do
            {

                 // Determine Vector
                 Cartesian_Vector vector = phys_vector (obj1->location,
                                                   obj2->location);

                 // Convert to Spherical Vector
                 Spherical_Vector sphr = phys_alias_transform (vector);;

                 //print_vector(obj1->name, sphr);
                 // Use radius value from spherical vector as distance
                 double distance = sphr.r;

                 // Replace radius with aggregate gravity force
                 sphr.r = phys_gravity(obj1->mass, obj2->mass, distance);

                 //print_vector("grav", sphr); //// DUBUG

                 // Add gravity as a force to first object
                 obj1->force_add(sphr);


                 // Invert Spherical Vector for object 2
                 sphr = phys_vector_invert(sphr);

                 // Add inverted gravity vector as a force to second object
                 obj2->force_add(sphr);

                 // Increment object 2
                 obj2 = obj2->next;

            } while (obj2 != NULL);

            // Increment object 1
            obj1 = obj1->next;

       } while (obj1 != NULL);

       obj1 = head;

       do // Calculate effects of force on objects
       {
            obj1->force_apply();
            obj1->accel_apply(time);
            obj1->velocity_apply(time);
            //print_object(*obj1);
            obj1 = obj1->next;
       } while (obj1 != NULL);

  }  // iterate_engine

} // Namespace
