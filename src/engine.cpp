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
            print_object(*obj1);

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

                 print_vector(obj1->name, sphr);
                 // Use radius value from spherical vector as distance
                 double distance = sphr.r;

                 // Replace radius with aggregate gravity force
                 sphr.r = phys_gravity(obj1->mass, obj2->mass, distance);

                 print_vector("grav", sphr); //// DUBUG

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
            print_object(*obj1);
            obj1 = obj1->next;
       } while (obj1 != NULL);
    
  }    
         
}

// BACKUP
#if 0
void iterate_engine (Object *head)
{
    Object *obj1 = head;
    Object *obj2 = NULL;

    //unsigned int calc = 0;

    do // Clear Force and Acceleration Vectors
    {
        obj1->force_clear();
        print_object(*obj1);
        // Increment object 1
        obj1 = obj1->next;
    } while (obj1 != NULL);


    obj1 = head;
    obj2 = NULL;
       
    do // Gravity Forces
    {
        if(obj1->next == NULL) break;
 
        //obj1->force_clear();

        obj2 = obj1->next;        

        do
        {

            // Determine Vector
	    Cartesian_Vector vector = phys_vector (obj1->location,
                                                   obj2->location);

            // Convert to Spherical Vector
            Spherical_Vector sphr = phys_alias_transform (vector);;

	    print_vector(obj1->name, sphr);
            // Use radius value from spherical vector as distance
            double distance = sphr.r;

            // Make sure mass of obj2 is non-trivial
            //if (obj2->mass > 1 && obj1->mass != 0) 
            //{

                // Replace radius with aggregate gravity force
                sphr.r = phys_gravity(obj2->mass, distance);

		print_vector("grav", sphr);
		
                // Add gravity as a force to first object
                obj1->force_add(sphr);
            }

            // Make sure mass of obj1 is non-trivial
            if (obj1->mass > 1 && obj2->mass != 0)
            {

                // Invert Spherical Vector for object 2
                sphr = phys_vector_invert(sphr);

                // Recalculate Gravity Acceleration Vector for object 2 from mass of objects 1
                sphr.r = phys_gravity(obj1->mass, distance);

                // Add inverted gravity vector as a force to second object
                obj2->force_add(sphr);
            }


            // Increment object 2
            obj2 = obj2->next;
            
	    //calc++; //for test, to be removed


        } while (obj2 != NULL);

        //context->apply_force(1);

        // Increment object 1
        obj1 = obj1->next;

    } while (obj1 != NULL);

    obj1 = head;
    
    do // Calculate effects of force on objects
    {
        obj1->force_apply();
	obj1->accel_apply(1);
	obj1->velocity_apply(1);
    
        // Increment object 1
        obj1 = obj1->next;
    } while (obj1 != NULL);
    
}    
#endif
