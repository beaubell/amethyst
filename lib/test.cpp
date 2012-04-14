/***********************************************************************
 Amethyst-lib
  - Library Test Functions, Implementations

 Authors (c):
 2006-2008 Beau V.C. Bellamy (beau@stellarnetservices.net)

 $Revision$
 $LastChangedDate$
 $LastChangedBy$
 ***********************************************************************/

#include <stdio.h>
#include <iostream>

#include "test.h"
#include "utility.h"
#include "universe.h"
#include "object.h"
#include "ship.h"
#include "physics.h"
#include "net.h"
#include "datatypes.h"

#ifdef __GNUG__
#pragma implementation
#endif

namespace amethyst {
namespace lib {

  void full_test(void)
  {
       full_test(false, false);
  };

  bool full_test(bool quiet, bool debug)
  {
       bool success = false;
       info_variable_size();

       test_units(quiet, debug);
       
       test_cartsphere_conv(quiet, debug);
       success = test_engine(quiet, debug);
       
       

//       unsigned short a = 30356, b;
//       printf("a: %#xh\n",a);
//       b = ntohs(a);
//       printf("b: %#xh\n", b);
       return success;
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

       Universe universe;

       universe.object_add(a);
       universe.object_add(b);
       universe.object_add(c);
       universe.object_add(d);

       a->name = "Object 1 (Sun)";
       a->mass = 5.972e24;

       b->name = "Object 2 (Earth)";
       b->mass = 1;
       b->location.z = 0;
       b->location.y = 6378150000.0;
       b->location.x = 0;

       c->name = "Object 3";
       c->mass = 7.35e22;
       c->location.z = 30002378000.0;
       c->location.y = 0;
       c->location.x = 0;

       d->name = "Object 4";
       d->mass = 9.2321223e23;
       d->location.x = 10000000;
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
           universe.iterate(.0001, 1);
           if (count1 > 100000) { printf("."); fflush (stdout); count1 = 0;};
           //if (count > 2500000) break;
           if (count >= 1000000) break;
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

       return 0; //XXX return meaningful success status
  } // test_engine

  void info_variable_size(void)
  {
       std::cout << "\nINFO: Variable sizes\n";
       std::cout << " bool       :  " << sizeof(bool)            << std::endl;
       std::cout << " char       :  " << sizeof(char)            << std::endl;
       std::cout << " short      :  " << sizeof(short)           << std::endl;
       std::cout << " int        :  " << sizeof(int)             << std::endl;
       std::cout << " long       :  " << sizeof(long)            << std::endl;
       std::cout << " long long  :  " << sizeof(long long)       << std::endl;
       std::cout << " float      :  " << sizeof(float)           << std::endl;
       std::cout << " double     :  " << sizeof(double)          << std::endl;
       std::cout << " lib:Vector :  " << sizeof(Cartesian_Vector)<< std::endl;
       std::cout << " lib:Quat   :  " << sizeof(Quaternion)      << std::endl;
       std::cout << " lib::Object:  " << sizeof(Object)          << std::endl;
       std::cout << " lib::Ship  :  " << sizeof(Ship)            << std::endl;
  }

  bool test_units(bool quiet, bool debug)
  {
    std::cout << "\nTest: Units and Dimensions\n";
    
      Location location = 9.0*si::meters;
      Time     time = 8.0*si::seconds;
      
      Velocity     velocity = location/time;
      Acceleration acceleration = velocity/time;
     
      std::cout << " Location                 : " << location << std::endl;
      std::cout << " Time                     : " << time     << std::endl;
      std::cout << " Velocity (Location/Time) : " << velocity << std::endl;
      std::cout << " Acceleration (Velo/Time) : " << acceleration << std::endl;
      
    return true;
  }


void test_rk4()
{
  cl_init();

  Object *a = new (Object);
  Object *b = new (Object);
  Object *c = new (Object);
  Object *d = new (Object);

  Universe universe;

  universe.object_add(a);
  universe.object_add(b);
  universe.object_add(c);
  universe.object_add(d);

  a->name = "Object 1 (Sun)";
  a->mass = 1.9891e30;  // AIP PDR 2002

  b->name = "Object 2 (Earth)";
  b->mass = 5.9743e24;  // PDR 2002
  placement_SimpleOrbit(*a, *b, 149.598e9);

  c->name = "Object 3 (Moon)";
  c->mass = 7.35e22;    // PDR 2002
  placement_SimpleOrbit(*b, *c, 384.400e6);

  d->name = "Object 4 (L1 Test Probe)";
  d->mass = 1;
  placement_L1(*a, *b, *d);

   std::cout << "Size of Universe: " << universe.object_count() << std::endl;
  
  universe.cl_setup();
  universe.cl_copytogpu();
  universe.cl_integrate();

  universe.object_del(a);
  universe.object_del(b);
  universe.object_del(c);
  universe.object_del(d);

  delete a;
  delete b;
  delete c;
  delete d;

}

} // namespace lib
} // namespace amethyst
