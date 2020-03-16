/***********************************************************************
 Amethyst-lib
  - Library Test Functions, Implementations

 Authors (c):
 2006-2020 Beau V.C. Bellamy (bellamy.beau@gmail.com)
 ***********************************************************************/

#include <stdio.h>

#include "test.h"
#include "utility.h"
#include "universe.h"
#include "object.h"
#include "ship.h"
#include "physics.h"
#include "net.h"
#include "datatypes.h"

namespace amethyst {
namespace lib {

bool full_test(ConsoleIO& io, bool quiet, bool debug) {

    bool success = false;
    info_variable_size(io);

    test_units(io, quiet, debug);
    
    test_cartsphere_conv(io, quiet, debug);
    success = test_engine(io, quiet, debug);
    
    return success;
}


bool test_cartsphere_conv(ConsoleIO& io, bool quiet, bool debug)
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


bool test_engine(ConsoleIO& io, bool quiet, bool /* debug unused*/)
{
    Object::sptr a = std::make_shared<Object>();
    Object::sptr b = std::make_shared<Object>();
    Object::sptr c = std::make_shared<Object>();
    Object::sptr d = std::make_shared<Object>();

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

    if (!quiet)
    {
        printf("\n");
        printf("Original Objects\n");
        print_vector(" OBJ-1", a->location);
        print_vector(" OBJ-2", b->location);
        print_vector(" OBJ-3", c->location);
        print_vector(" OBJ-4", d->location);

        printf("\nEngine Running...\n");
    } // if (!quiet)

    for (;;)
    {
        count++;
        count1++;
        universe.iterate(.0001, 1);
        if (!quiet && count1 > 100000) { printf("."); fflush (stdout); count1 = 0;};
        //if (count > 2500000) break;
        if (count >= 1000000) break;
    }

    if (!quiet)
    {
        printf(" %d: Iterations have passed\n\n", count);

        printf("Processed Objects\n");
        print_vector(" OBJ-1", a->location);
        print_vector(" OBJ-2", b->location);
        print_vector(" OBJ-3", c->location);
        print_vector(" OBJ-4", d->location);

        printf("\nDISTANCE: %le, GRAVITY: %le\n", phys_distance(a->location, b->location), phys_gravity(*a, *b));
    } // if (!quirt)

    return false; //XXX return meaningful success status
} // test_engine

void info_variable_size(ConsoleIO& io)
{
    io.out << "\nINFO: Variable sizes\n";
    io.out << " bool       :  " << sizeof(bool)            << '\n';
    io.out << " char       :  " << sizeof(char)            << '\n';
    io.out << " short      :  " << sizeof(short)           << '\n';
    io.out << " int        :  " << sizeof(int)             << '\n';
    io.out << " long       :  " << sizeof(long)            << '\n';
    io.out << " long long  :  " << sizeof(long long)       << '\n';
    io.out << " float      :  " << sizeof(float)           << '\n';
    io.out << " double     :  " << sizeof(double)          << '\n';
    io.out << " lib:Vector :  " << sizeof(Cartesian_Vector)<< '\n';
    io.out << " lib:Quat   :  " << sizeof(Quaternion)      << '\n';
    io.out << " lib::Object:  " << sizeof(Object)          << '\n';
    io.out << " lib::Ship  :  " << sizeof(Ship)            << '\n';
}

bool test_units(ConsoleIO& io, bool quiet, bool /* debug unused*/)
{
    if (!quiet)
        io.out << "\nTest: Units and Dimensions\n";

    Location location = 9.0*si::meters;
    Time     time = 8.0*si::seconds;
    
    Velocity     velocity = location/time;
    Acceleration acceleration = velocity/time;
    
    if (!quiet)
    {
        io.out << " Location                 : " << location << '\n';
        io.out << " Time                     : " << time     << '\n';
        io.out << " Velocity (Location/Time) : " << velocity << '\n';
        io.out << " Acceleration (Velo/Time) : " << acceleration << '\n';
    } // if (!quiet)

return true;
}


void test_rk4(ConsoleIO& io)
{
  cl_init();

  Object::sptr a = std::make_shared<Object>();
  Object::sptr b = std::make_shared<Object>();
  Object::sptr c = std::make_shared<Object>();
  Object::sptr d = std::make_shared<Object>();

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

  io.out << "Size of Universe: " << universe.object_count() << '\n';
  
  universe.cl_setup();
  universe.cl_copytogpu();
  universe.cl_integrate();

  //universe.cl_load_history("history_dump.h5");
  universe.cl_save_history("history_dump1.h5");

  universe.object_del(a);
  universe.object_del(b);
  universe.object_del(c);
  universe.object_del(d);

}

} // namespace lib
} // namespace amethyst
