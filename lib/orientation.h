/***************************************************************************
 *   Copyright (C) 2004 by Beau V.C. Bellamy                               *
 *   beau@borealisbroadband.net                                            *
 ***************************************************************************/

#ifndef AMETHYST_ORIENTATION_H
#define AMETHYST_ORIENTATION_H

#include "vector.h"

namespace amethyst {

  // Euler Data Type and Support Functions
  class Euler {

       public:

       double x;
       double y;
       double z;

       inline Euler() : x(0), y(0), z(0) {};
       inline Euler(const double &xx, const double &yy, const double &zz) : x(xx), y(yy), z(zz){};
       Euler(const Euler &right) : x(right.x), y(right.y), z(right.z) {};

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
       Quaternion(const Quaternion&);
       Quaternion(const Euler&);

       void  normalize(void);

       Cartesian_Vector  GetVector(void);
       Quaternion        Bar(void) { return Quaternion(w, -x, -y, -z); };
       Quaternion        scale(const double &s);
       double            length(void);
       double            dot   (const Quaternion &right) const;
       void              slerp (const Quaternion &left, Quaternion &right, float t);
       //Cartesian_Vector GetAxis(void);
       void              setIdentity(void) {w=1; x=y=z=0; };
       bool              isIdentity() {return ( w==1 && x==0 && y==0 && z==0); };

       //const Quaternion& operator = (Quaternion&);
       Quaternion operator*(double scale) const;
       Quaternion operator+(const Quaternion &b) const;

       const Quaternion& operator *= (Quaternion&);
       const Quaternion& operator *= (double);
       //const Quaternion& operator ~  (void) { return Quaternion( w , -x, -y, -z); };
       };

  const Quaternion operator* (const Quaternion &left, const Quaternion &right);
  const Quaternion operator* (const Quaternion &q, const Cartesian_Vector &v);
  const Quaternion operator* (const Cartesian_Vector &v, const Quaternion &q);
  //const Quaternion operator* (const Quaternion &q, const double &s);

  double     dot   (const Quaternion &left, const Quaternion &right);


/// Rotate vector around a unit quaternion
Cartesian_Vector QVRotate(Quaternion &q, const Cartesian_Vector &v);

}
#endif  /* AMETHYST_ORIENTATION_H */
