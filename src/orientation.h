/***************************************************************************
 *   Copyright (C) 2004 by Beau V.C. Bellamy                               *
 *   beau@borealisbroadband.net                                            *
 ***************************************************************************/

#ifndef AMETHYST_ORIENTATION_H
#define AMETHYST_ORIENTATION_H
 
#include "vector.h"
 
namespace amethyst { 

  // Euler Data Type and Support Functions
  class Euler : public Cartesian_Vector {
     
       public:
       
       //float x;
       //float y;
       //float z;
       
       //inline Euler() : x(0), y(0), z(0) {};
       //inline Euler(const float &xx, const float &yy, const float &zz) : x(xx), y(yy), z(zz){};
       //Euler(const Euler&);

       //void Set_Identity(void);
       
       //const Euler& operator = (Euler&);
       
       //const Euler& operator *= (Euler&);
       };

  const Euler operator* (const Euler &left, const Euler &right);

       
  // Quaternion Data Type and Support Functions
  class Quaternion {
     
       public:
    
       float w;
       float x;
       float y;
       float z;
       
       inline Quaternion() : w(0), x(0), y(0), z(0) {};
       inline Quaternion(const float &ww, const float &xx, const float &yy, const float &zz) : w(ww), x(xx), y(yy), z(zz){};
       Quaternion(const Quaternion&);
       Quaternion(const Euler&);

       void Normalize(void);
       //void Set_Identity(void);
       
       //const Quaternion& operator = (Quaternion&);
       
       const Quaternion& operator *= (Quaternion&);
       };

  const Quaternion operator* (const Quaternion &left, const Quaternion &right);
}
#endif  /* AMETHYST_ORIENTATION_H */
