/*
   Physics Library Performance Test Functions
   Author: Beau V.C. Bellamy
*/

// No great genius has ever existed without some touch of madness. 
//                                                         - Aristotle

#include <stdio.h>

#include "object.h"
#include "physics.h"

void loop (Object *head);

int main(int argc, char** argv)
{
    Object *a = new (Object);
    Object *b = new (Object);
    Object *c = new (Object);
    Object *d = new (Object);

    a->next = b;
    b->next = c;
    c->next = d;

    a->mass = 5.972e24;
    b->mass = 1;
    b->location.z = 0;
    b->location.y = 6378150;
    b->location.x = 0;

    c->mass = 7.35e22;
    c->location.z = 30002378;
    c->location.y = 0;
    c->location.x = 0;

    //Spherical_Vector vector;
    //Cartesian_Vector interrim;
    //Cartesian_Vector test;
  
    int count = 0;

    for (;;) 
    {
        count++;
        loop (a);
        if (count > 2500000) break;
    }

    //printf("\nVECTOR a:%le p:%le r:%le ", vector.a, vector.p, vector.r);
    //printf("\nTEST x:%le y:%le z:%le ", test.x, test.y, test.z);

    //printf("\nDISTANCE: %le, GRAVITY: %le\n", phys_distance(&(a->location), &(b->location)), phys_gravity(a, b));

    //loop (a);

    delete d;
    delete c;
    delete b;
    delete a;

    return 0;
}


void loop (Object *head) {

    Object *obj1 = head;
    Object *obj2 = NULL;

    unsigned int calc = 0;

    do
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

            // Use radius value from spherical vector as distance
            double distance = sphr.r;

            // Make sure mass of obj2 is non-trivial
            if (obj2->mass > 1 && obj1->mass != 0) 
	    {

                // Replace radius with aggregate gravity force
                sphr.r = phys_gravity(obj2->mass, distance);

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
            
	    calc++; //for test, to be removed


        } while (obj2 != NULL);

        //context->apply_force(1);

        // Increment object 1
        obj1 = obj1->next;

    } while (obj1 != NULL);

}    
