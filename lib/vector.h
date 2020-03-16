#pragma once
/***********************************************************************
 Amethyst-lib
  - Vector Class Objects Declarations

 Authors (c):
 2006-2020 Beau V.C. Bellamy (bellamy.beau@gmail.com)
 ***********************************************************************/

#include <ostream>
#include "yaml-cpp/node/node.h"

namespace amethyst {
namespace lib {

  // Cartesian_Vector Data Type and Support Functions
  class Cartesian_Vector {

     public:
       double x;
       double y;
       double z;
       double w; // For GPU packing

       inline Cartesian_Vector() : x(0), y(0), z(0), w(0) {};
       inline Cartesian_Vector(const double &xx, const double &yy, const double &zz) : x(xx), y(yy), z(zz), w(0){};
       Cartesian_Vector(const Cartesian_Vector &right) : x(right.x), y(right.y), z(right.z), w(0){};

       void    clear(void);

       double  magnitude(void) const;
       void    normalize(void);
       void    reverse(void);

       YAML::Node toYAML() const;
       void fromYAML(YAML::Node vec);


       //const Cartesian_Vector& operator = (Cartesian_Vector&);

       const Cartesian_Vector& operator += (const Cartesian_Vector&);
       const Cartesian_Vector& operator -= (const Cartesian_Vector&);
       const Cartesian_Vector& operator *= (const double&);
       const Cartesian_Vector& operator /= (const double&);

       Cartesian_Vector operator -(void);
       Cartesian_Vector& operator =(const Cartesian_Vector &right);
       friend std::ostream& operator<<(std::ostream& os, const Cartesian_Vector& dt);
  };

  typedef Cartesian_Vector Cartesian_Coord;

  const Cartesian_Vector operator+ (const Cartesian_Vector &left, const Cartesian_Vector &right);
  const Cartesian_Vector operator- (const Cartesian_Vector &left, const Cartesian_Vector &right);
  const Cartesian_Vector operator* (const Cartesian_Vector &left, const double &scalar);
  const Cartesian_Vector operator/ (const Cartesian_Vector &left, const double &scalar);

  bool  operator == (const Cartesian_Vector &left, const Cartesian_Vector &right);
  std::ostream& operator<<(std::ostream& os, const Cartesian_Vector& cv);


  // Spherical_Vector Data Type and Support Functions
  class Spherical_Vector {

       public:
       // Azimuthal angle in the xy plane from the z axis
       // radians, 0 <= a < 2PI
       double a;

       // Polar angle from the z axis
       // radians, 0 <= p <= PI
       double p;

       // radius, distance from the center
       double r;

       inline Spherical_Vector() : a(0), p(0), r(0) {};
       inline Spherical_Vector(const double &aa, const double &pp, const double &rr) : a(aa), p(pp), r(rr) {};
       Spherical_Vector(const Spherical_Vector &right) : a(right.a), p(right.p), r(right.r) {};
       virtual ~Spherical_Vector(void) {};

       void  clear(void);

       YAML::Node toYAML() const;
       void fromYAML(YAML::Node node);

       Spherical_Vector& operator = (const Spherical_Vector &right);

       const Spherical_Vector& operator += (Spherical_Vector&);
       const Spherical_Vector& operator -= (Spherical_Vector&);
       const Spherical_Vector& operator *= (double&);
       const Spherical_Vector& operator /= (double&);
       friend std::ostream& operator<<(std::ostream& os, const Spherical_Vector& dt);
  };

  typedef Spherical_Vector Spherical_Coord;

  const Spherical_Vector operator+ (const Spherical_Vector &left, const Spherical_Vector &right);
  const Spherical_Vector operator- (const Spherical_Vector &left, const Spherical_Vector &right);
  const Spherical_Vector operator* (const Spherical_Vector &left, const double &scalar);
  const Spherical_Vector operator/ (const Spherical_Vector &left, const double &scalar);

  //const Spherical_Vector& operator= (const Spherical_Vector &left, const Cartesian_Vector &right);
  //const Cartesian_Vector& operator= (const Cartesian_Vector &left, const Spherical_Vector &right);

} // namespace lib
} // namespace amethyst
