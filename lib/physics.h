#ifndef AMETHYST_LIB_PHYSICS_H
#define AMETHYST_LIB_PHYSICS_H

/***********************************************************************
 Amethyst-lib
  - Physics Functions, Declarations

 Authors (c):
 2006-2008 Beau V.C. Bellamy (beau@stellarnetservices.net)

 $Revision$
 $LastChangedDate$
 $LastChangedBy$
 ***********************************************************************/

#include "object.h"

#ifdef __GNUG__
#pragma interface
#endif

namespace amethyst {
namespace lib {

// Constants

  // Gravitational Constant, Defined in physics.cpp   (6.672E-11)
  extern Grav_Const G;

  // Speed of Light        , Defined in physics.cpp   (2.99792458E8 meters per second)
  extern Velocity C;

  // PI
  //extern double PI;


// Functions

  // Compute Distance between 'Object A' and 'Object B'
  template <typename T>
  T phys_distance (const Cartesian_Vector<T> &a, const Cartesian_Vector<T> &b)
  {

      Cartesian_Vector<T> vector;
      vector = phys_vector (a, b);

      return sqrt( pow(vector.x,2)+
                   pow(vector.y,2)+
                   pow(vector.z,2) );
  }

  // Compute Gravity between 'Object A' and 'Object B'
  Force phys_gravity (const Object &a, const Object &b);

  // Compute Gravity for mass of one object
  Force phys_gravity (const Mass &mass_a, const Location &distance);

  // Conpute Gravity for mass of two objects
  Force phys_gravity (const Mass &mass_a, const Mass &mass_b, const Location &distance);

  // Compute Cartisian Vector from Cartesian Coordinates
  template <typename T>
  Cartesian_Vector<T> phys_vector (const Cartesian_Vector<T> &a,
                                const Cartesian_Vector<T> &b)
  { 
      Cartesian_Vector<T> output;

      output.x = b.x - a.x;
      output.y = b.y - a.y;
      output.z = b.z - a.z;

      return output;
  }
 
  // Invert Spherical Vector
  template <typename T>
  Spherical_Vector<T> phys_vector_invert (const Spherical_Vector<T> &a)
  {
      Spherical_Vector<T> output;

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

  template <typename T>
  Spherical_Vector<T> phys_alias_transform (const Cartesian_Vector<T> &a)
  {

      Cartesian_Vector<T> zero;

      T radius = phys_distance (zero, a);
      return Spherical_Vector<T> ( atan2(a.y, a.x), acos(a.z/radius), radius);

      //return output;
  }

  template <typename T>
  Cartesian_Vector<T> phys_alias_transform (const Spherical_Vector<T> &a)
  {

      Cartesian_Vector<T> output( (a.r)*cos(a.a)*sin(a.p), (a.r)*sin(a.a)*sin(a.p), (a.r)*cos(a.p) );

      return output;
  }
      
  // Perform alibi transformation
  template <typename T>
  Cartesian_Vector<T> phys_alibi_transform (const Cartesian_Vector<T> &a,
                                           const Cartesian_Vector<T> &v)
  {
      Cartesian_Vector<T> output;

      output.x = a.x + v.x;
      output.y = a.y + v.y;
      output.z = a.z + v.z;

      return output;
  }
					   

  // Perform alibi transformation with multiplier
  template <typename T>
  Cartesian_Vector<T> phys_alibi_transform (const Cartesian_Vector<T> &a,
                                        const Cartesian_Vector<T> &v,
                                        const Scalar& modifier)
  {

      Cartesian_Vector<T> output;

      output.x = (a.x + v.x) * modifier;
      output.y = (a.y + v.y) * modifier;
      output.z = (a.z + v.z) * modifier;

      return output;
  }
  
} // namespace lib
} // namespace amethyst

#endif  /* AMETHYST_LIB_PHYSICS_H */
