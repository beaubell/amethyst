#ifndef AMETHYST_LIB_PHYSICS_H
#define AMETHYST_LIB_PHYSICS_H

/***********************************************************************
 Amethyst-lib
  - Physics Functions, Declarations

 Authors (c):
 2006-2023 Beau V.C. Bellamy (bellamy.beau@gmail.com)

 ***********************************************************************/

#include "object.h"

#define BOOST_MATH_DOMAIN_ERROR_POLICY throw_on_error

namespace amethyst::lib {

// Constants

  // Gravitational Constant, Defined in physics.cpp   (6.672E-11)
  extern const double G;

  // Speed of Light        , Defined in physics.cpp   (2.99792458E8 meters per second)
  extern const double C;

  // PI
  //extern double PI;


// Functions

  // Compute Distance between 'Object A' and 'Object B'
  double phys_distance (const Cartesian_Coord &a, const Cartesian_Coord &b);

  // Compute Gravity between 'Object A' and 'Object B'
  double phys_gravity (const Object &a, const Object &b);

  // Compute Gravity for mass of one object
  double phys_gravity (const double &mass_a, const double &distance);

  // Conpute Gravity for mass of two objects
  double phys_gravity (const double &mass_a, const double &mass_b, const double &distance);

  // Compute Cartisian Vector from Cartesian Coordinates
  Cartesian_Vector phys_vector (const Cartesian_Coord &a,
                                const Cartesian_Coord &b);

  // Invert Spherical Vector
  Spherical_Vector phys_vector_invert (const Spherical_Vector &a);

  // Perform alias transformation from Cartesian to Spherical
  Spherical_Vector phys_alias_transform (const Cartesian_Coord &a);

  // Perform alias transformation from Spherical to Cartesian
  Cartesian_Vector phys_alias_transform (const Spherical_Vector &a);

  // Perform alibi transformation
  Cartesian_Coord phys_alibi_transform (const Cartesian_Coord &a,
                                        const Cartesian_Vector &v);

  // Perform alibi transformation with multiplier
  Cartesian_Coord phys_alibi_transform (const Cartesian_Coord &a,
                                        const Cartesian_Vector &v,
                                        double modifier);

  void placement_SimpleOrbit(const Object &primary, Object &satellite, double distance);
  void placement_L1(const Object &primary, const Object &satellite, Object &L1);
  void placement_L2(const Object &primary, const Object &satellite, Object &L2);
  double distance_L1(const Object &primary, const Object &satellite, Object &probe);

} // namespace amethyst::lib

#endif  /* AMETHYST_LIB_PHYSICS_H */
