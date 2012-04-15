/***********************************************************************
 Amethyst-lib
  - Physics Functions, Implementations

 Authors (c):
 2006-2008 Beau V.C. Bellamy (beau@stellarnetservices.net)

 $Revision$
 $LastChangedDate$
 $LastChangedBy$
 ***********************************************************************/

#include <math.h>

#include "physics.h"

#ifdef __GNUG__
#pragma implementation
#endif

#ifdef WIN32
#define M_PI  3.14159265358979323846
#define M_PIl 3.1415926535897932384626433832795029L
#endif

namespace amethyst {
namespace lib {

// Big 'G', The gravitational constant, also The constant of proportionality.
double G = 6.6738480e-11; // N m^2 kg^-2
// Speed of Light
double C = 2.99792458e8;  // m/s

double phys_distance (const Cartesian_Coord &a, const Cartesian_Coord &b)
{

  Cartesian_Vector vector;
  vector = phys_vector (a, b);

  return sqrt( pow(vector.x,2)+
                pow(vector.y,2)+
                pow(vector.z,2) );
}



double phys_gravity (const Object &a, const Object &b)
{

  double distance = phys_distance (a.location, b.location);

  return ((G*(a.mass)*(b.mass)) / pow(distance, 2));
}


double phys_gravity (const double &mass_a, const double &distance)
{
  return ((G*(mass_a)) / pow(distance, 2));
}


double phys_gravity (const double &mass_a, const double &mass_b, const double &distance)
{
  return ((G*(mass_a)*(mass_b)) / pow(distance, 2));
}


Spherical_Vector phys_alias_transform (const Cartesian_Coord &a)
{

  Cartesian_Vector zero;
  //Spherical_Vector output;

  //zero.x = 0;
  //zero.y = 0;
  //zero.z = 0;

  //output.r = phys_distance (zero, a);
  //output.a = atan2(a.y, a.x);
  //output.p = acos(a.z/output.r);

  double radius = phys_distance (zero, a);
  return Spherical_Vector ( atan2(a.y, a.x), acos(a.z/radius), radius);

  //return output;
}


Cartesian_Vector phys_alias_transform (const Spherical_Vector &a)
{

  //Cartesian_Vector output;

  //output.x = (a.r)*cos(a.a)*sin(a.p);
  //output.y = (a.r)*sin(a.a)*sin(a.p);
  //output.z = (a.r)*cos(a.p);

  Cartesian_Vector output( (a.r)*cos(a.a)*sin(a.p), (a.r)*sin(a.a)*sin(a.p), (a.r)*cos(a.p) );

  return output;
}


Cartesian_Vector phys_vector (const Cartesian_Coord &a,
                            const Cartesian_Coord &b)
{

  Cartesian_Vector output;

  output.x = b.x - a.x;
  output.y = b.y - a.y;
  output.z = b.z - a.z;

  return output;
}


Spherical_Vector phys_vector_invert (const Spherical_Vector &a)
{
  Spherical_Vector output;

  // invert Azimuthal angle
  if (a.a > M_PI)
  {
      output.a = a.a - M_PI;
    } else {
      output.a = a.a + M_PI;
  }

  // invert Polar angle
  output.p = M_PI - a.p;

  // pass radius verbatim
  output.r = a.r;

  return output;
}


Cartesian_Coord phys_alibi_transform (const Cartesian_Coord &a,
                                      const Cartesian_Vector &v)
{
  Cartesian_Coord output;

  output.x = a.x + v.x;
  output.y = a.y + v.y;
  output.z = a.z + v.z;

  return output;
}


Cartesian_Coord phys_alibi_transform (const Cartesian_Coord &a,
                                      const Cartesian_Vector &v,
                                      const double modifier)
{

  Cartesian_Coord output;

  output.x = (a.x + v.x) * modifier;
  output.y = (a.y + v.y) * modifier;
  output.z = (a.z + v.z) * modifier;

  return output;
}

void placement_SimpleOrbit(const Object &primary, Object &satellite, double distance)
{
  satellite.location = primary.location;
  satellite.location.x += distance;

  satellite.velocity = primary.velocity;
  double grav_acc = G*primary.mass/pow(distance,2);
  satellite.velocity.y += sqrt(grav_acc * distance);
}


void placement_L1(const Object &primary, const Object &satellite, Object &L1)
{

  /// Find Location of L1 Point
  Cartesian_Vector to_body1 = primary.location - satellite.location;
  double distance = to_body1.magnitude();
  to_body1.normalize();

  //double distanceL1 = distance*pow(satellite.mass/(3.0*primary.mass),1.0/3.0);
  double distanceL1 = distance*cbrt(satellite.mass/(3.0*(primary.mass + satellite.mass)));
  //double distanceL1 = distance*sqrt(satellite.mass)/(sqrt(primary.mass) + sqrt(satellite.mass));
  L1.location = satellite.location + to_body1*distanceL1;

  /// Find Velocity of L1 Point
  double disL1ratio = distanceL1/distance;
  L1.velocity = (primary.velocity - satellite.velocity)*disL1ratio + satellite.velocity;
  //L1.velocity = primary.velocity*(-disL1ratio) + satellite.velocity*(1.0+disL1ratio);

}


void placement_L2(const Object &primary, const Object &satellite, Object &L2)
{

  /// Find Location of L1 Point
  Cartesian_Vector to_body1 = primary.location - satellite.location;
  double distance = to_body1.magnitude();
  to_body1.normalize();

  //double distanceL2 = distance*pow(satellite.mass/(3.0*primary.mass),1.0/3.0);
  double distanceL2 = distance*cbrt(satellite.mass/(3.0*(primary.mass + satellite.mass)));
  L2.location = satellite.location - to_body1*distanceL2;

  /// Find Velocity of L1 Point
  double disL2ratio = distanceL2/distance;
  //L2.velocity = (primary.velocity - satellite.velocity)*disL2ratio + satellite.velocity;
  L2.velocity = primary.velocity*(-disL2ratio) + satellite.velocity*(1.0+disL2ratio);

}


double distance_L1(const Object &primary, const Object &satellite, Object &probe)
{
  Object L1;
  placement_L1(primary, satellite, L1);

  return (probe.location - L1.location).magnitude();
}


double distance_L2(const Object &primary, const Object &satellite, Object &probe)
{
  Object L2;
  placement_L2(primary, satellite, L2);

  return (probe.location - L2.location).magnitude();
}

} // namespace lib
} // namespace amethyst
