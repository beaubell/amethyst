/*
   Physics Library Performance Test Functions, Definitions
   Amethyst Physics Library (c) 2003
   Author: Beau V.C. Bellamy
*/

// 
// 

#include <stdio.h>

#include "test.h"
#include "utility.h"
#include "engine.h"
#include "object.h"
#include "physics.h"

#ifdef __GNUG__
#pragma implementation
#endif

namespace amethyst {

  void full_test(void)
  {

       Object *a = new (Object);
       Object *b = new (Object);
       Object *c = new (Object);
       Object *d = new (Object);

       functional_test();
    
       a->next = b;
       b->next = c;
       c->next = d;

       a->name = "Object 1 (Sun)";
       a->mass = 5.972e24;
    
       b->name = "Object 2 (Earth)";  
       b->mass = 1;
       b->location.z = 0;
       b->location.y = 6378150;
       b->location.x = 0;

       c->name = "Object 3";
       c->mass = 7.35e22;
       c->location.z = 30002378;
       c->location.y = 0;
       c->location.x = 0;
    
       d->name = "Object 4";
       d->mass = 9.2321223e23;
       d->location.x = 10000;
       d->location.y = 0;
       d->location.z = 0;

       int count  = 0;
       int count1 = 0;

       printf("\n");
       printf("Original Objects\n");
       print_vector(" OBJ-1", a->location);
       print_vector(" OBJ-2", b->location);
       print_vector(" OBJ-3", c->location);
       print_vector(" OBJ-4", d->location);

       printf("\nEngine Running...\n");
    
       for (;;) 
       {
           count++;
           count1++;
           iterate_engine (a,1);
           if (count1 > 100000) { printf("."); fflush (stdout); count1 = 0;};
           //if (count > 2500000) break;
           if (count > 0) break;
       }

       printf(" %d: Iterations have passed\n\n", count);
    
       printf("Processed Objects\n");
       print_vector(" OBJ-1", a->location);
       print_vector(" OBJ-2", b->location);
       print_vector(" OBJ-3", c->location);
       print_vector(" OBJ-4", d->location);

       printf("\nDISTANCE: %le, GRAVITY: %le\n", phys_distance(a->location, b->location), phys_gravity(*a, *b));

       delete d;
       delete c;
       delete b;
       delete a;
  }
  
  void functional_test(void)
  {
       Cartesian_Vector a(100,100,100);
     
       printf("\nCartesian->Sphercial->Cartesian Test...\n");
       print_vector(" Test Vector", a);
     
       Spherical_Vector sphr = phys_alias_transform (a);
     
       print_vector(" Test Vector", sphr);
     
       Cartesian_Vector b = phys_alias_transform (sphr);
     
       print_vector(" Test Vector", a);

     
  }

}
