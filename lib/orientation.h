#pragma once
/***********************************************************************
 Amethyst-lib
  - Orientation Class Objects Declarations

 Authors (c):
 2006-2020 Beau V.C. Bellamy (bellamy.beau@gmail.com)
 ***********************************************************************/

#include <ostream>

#include "vector.h"
#include "yaml-cpp/node/node.h"

namespace amethyst::lib {

  // Euler Data Type and Support Functions
  class Euler {

       public:

       double x;
       double y;
       double z;

       inline Euler() : x(0), y(0), z(0) {};
       inline Euler(const double &xx, const double &yy, const double &zz) : x(xx), y(yy), z(zz){};
       Euler(const Euler &right) : x(right.x), y(right.y), z(right.z) {};
       
       [[nodiscard]] YAML::Node toYAML() const;
       void fromYAML(const YAML::Node& vec);

       //void Set_Identity(void);

       //const Euler& operator = (Euler&);

       //const Euler& operator *= (Euler&);
       };

  const Euler operator* (const Euler &left, const Euler &right);


  // Quaternion Data Type and Support Functions
  class Quaternion {

       public:

       double w;
       double x;
       double y;
       double z;

       inline Quaternion() : w(1), x(0), y(0), z(0) {};
       inline Quaternion(const double &ww, const double &xx, const double &yy, const double &zz) : w(ww), x(xx), y(yy), z(zz){};
       Quaternion(const Quaternion&) = default;
       Quaternion(Quaternion&&) = default;
       explicit Quaternion(const Euler&);
       ~Quaternion() = default;

       void  normalize();
       
       [[nodiscard]] YAML::Node toYAML() const;
       void fromYAML(const YAML::Node& vec);

       [[nodiscard]] Cartesian_Vector  GetVector() const;
       [[nodiscard]] Quaternion        Bar() const { return {w, -x, -y, -z}; };
       [[nodiscard]] Quaternion        scale(const double &s) const;
       [[nodiscard]] double            length() const;
       [[nodiscard]] double            dot   (const Quaternion &right) const;
       void               slerp (const Quaternion &left, Quaternion &right, double t);
       //Cartesian_Vector GetAxis(void);
       void               setIdentity() {w=1; x=y=z=0; };
       [[nodiscard]] bool isIdentity() const {return ( w==1 && x==0 && y==0 && z==0); };

       Quaternion& operator=(const Quaternion&) = default;
       Quaternion& operator=(Quaternion&&)      = default;
       Quaternion operator*(double scale) const;
       Quaternion operator+(const Quaternion &b) const;

       const Quaternion& operator *= (Quaternion&);
       const Quaternion& operator *= (double);
       //const Quaternion& operator ~  (void) { return Quaternion( w , -x, -y, -z); };
       friend std::ostream& operator<<(std::ostream& os, const Quaternion& q);
       };

  const Quaternion operator* (const Quaternion &left, const Quaternion &right);
  const Quaternion operator* (const Quaternion &q, const Cartesian_Vector &v);
  const Quaternion operator* (const Cartesian_Vector &v, const Quaternion &q);
  //const Quaternion operator* (const Quaternion &q, const double &s);

  double     dot   (const Quaternion &left, const Quaternion &right);


/// Rotate vector around a unit quaternion
Cartesian_Vector QVRotate(Quaternion &q, const Cartesian_Vector &v);

} // namespace amethyst::lib
