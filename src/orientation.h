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
       Euler(const Euler&);

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
       
       inline Quaternion() : w(0), x(0), y(0), z(0) {};
       inline Quaternion(const double &ww, const double &xx, const double &yy, const double &zz) : w(ww), x(xx), y(yy), z(zz){};
       Quaternion(const Quaternion&);
       Quaternion(const Euler&);

       void Normalize(void);

       Cartesian_Vector GetVector(void);
       Quaternion       Bar(void) { return Quaternion(w, -x, -y, -z); };
       //Cartesian_Vector GetAxis(void);
       //void Set_Identity(void);
       
       //const Quaternion& operator = (Quaternion&);
       
       const Quaternion& operator *= (Quaternion&);
       const Quaternion& operator ~  (void) { return Quaternion( w , -x, -y, -z); };
       };

  const Quaternion operator* (const Quaternion &left, const Quaternion &right);
  const Quaternion operator* (const Quaternion &q, const Cartesian_Vector &v);
  const Quaternion operator* (const Cartesian_Vector &v, const Quaternion &q);

}
#endif  /* AMETHYST_ORIENTATION_H */
