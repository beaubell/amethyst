/*
   Physics Subroutines, Definitions
   Amethyst Physics Library (c) 2003
   Author: Beau V.C. Bellamy
*/

// Dignity does not consist in possessing honors, but in deserving them.
//                                                             - Aristotle

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

  // Big 'G', The gravitational constant, also The constant of proportionality.
  double G = 6.672e-11;     // N m^2 kg^-2
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

} // namespace
