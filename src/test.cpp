/*
   Physics Library Performance Test Functions, Definitions
   Amethyst Physics Library (c) 2003
   Author: Beau V.C. Bellamy
*/

// 
// 

#include <stdio.h>
#include <iostream>

#include "test.h"
#include "utility.h"
#include "engine.h"
#include "object.h"
#include "physics.h"
#include "net.h"

#ifdef __GNUG__
#pragma implementation
#endif

namespace amethyst {

  void full_test(void) 
  {
       full_test(false, false);
  };

  bool full_test(bool quiet, bool debug)
  {
       info_variable_size();
       test_cartsphere_conv(quiet, debug);
       test_engine(quiet, debug);

//       unsigned short a = 30356, b;
//       printf("a: %#xh\n",a);
//       b = ntohs(a);
//       printf("b: %#xh\n", b);
  }

  bool test_cartsphere_conv(bool quiet, bool debug)
  {
       Cartesian_Vector a(100,100,100);
       Spherical_Vector sphr;
       Cartesian_Vector b;

       if (!quiet) printf("\nCartesian->Sphercial->Cartesian Test...");

       sphr = phys_alias_transform (a);
       b    = phys_alias_transform (sphr);

       bool success = (b == a);

       if (!quiet && success)
           printf("PASS\n");
         else
         {
           printf("FAIL !!! \n");
           debug = true; 
         }

       if (!quiet && debug) {
           print_vector(" DBG: Test Vector", a);
           print_vector(" DBG: Test Vector", sphr);
           print_vector(" DBG: Test Vector", b);
       }

       return success;
  }

  bool test_engine(bool quiet, bool debug)
  {
       Object *a = new (Object);
       Object *b = new (Object);
       Object *c = new (Object);
       Object *d = new (Object);

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
           if (count >= 2) break;
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
  } // test_engine

  void info_variable_size(void)
  {
       std::cout << "\nINFO: Variable sizes\n";
       std::cout << " bool       :  " << sizeof(bool) * 8        << std::endl;
       std::cout << " char       :  " << sizeof(char) * 8        << std::endl;
       std::cout << " short      :  " << sizeof(short) * 8       << std::endl;
       std::cout << " int        :  " << sizeof(int) * 8         << std::endl;
       std::cout << " long       :  " << sizeof(long) * 8        << std::endl;
       std::cout << " long long  :  " << sizeof(long long) * 8   << std::endl;
       std::cout << " float      :  " << sizeof(float) * 8       << std::endl;
       std::cout << " double     :  " << sizeof(double) * 8      << std::endl;
       std::cout << " long double:  " << sizeof(long double) * 8 << std::endl;
  }

} // Namespace Amethyst
