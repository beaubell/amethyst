/***************************************************************************
 *   Copyright (C) 2004 by Beau V.C. Bellamy                               *
 *   beau@borealisbroadband.net                                            *
 ***************************************************************************/

 #include <math.h>
 #include "object.h"
 #include "orientation.h"

namespace amethyst {

  Euler::Euler(const Euler &old) {

       x = old.x;
       y = old.y;
       z = old.z;
       }


  Quaternion::Quaternion(const Quaternion &old) {

       w = old.w;
       x = old.x;
       y = old.y;
       z = old.z;
       }


  Quaternion::Quaternion(const Euler &old) {

       //Qx = [ cos(a/2), (sin(a/2), 0, 0)]
       //Qy = [ cos(b/2), (0, sin(b/2), 0)]
       //Qz = [ cos(c/2), (0, 0, sin(c/2))]


       Quaternion Qx( cosf(old.x/2.0f), (sinf(old.x/2.0f)), 0, 0 );
       Quaternion Qy( cosf(old.y/2.0f), 0, (sinf(old.y/2.0f)), 0 );
       Quaternion Qz( cosf(old.z/2.0f), 0, 0, (sinf(old.z/2.0f)) );

       *this = Qz * Qy * Qx;
       //*this = Qx;
       }


   //const Quaternion& Quaternion::operator = (Quaternion& right) {
   //}


   static double length(Quaternion &quat){

       return sqrt(quat.x * quat.x + quat.y * quat.y +
              quat.z * quat.z + quat.w * quat.w);
       }


   void Quaternion::normalize(void) {

       double L = length(*this);

       x /= L;
       y /= L;
       z /= L;
       w /= L;
       }

   Cartesian_Vector  Quaternion::GetVector(void) {

       return Cartesian_Vector(x,y,z);
       }

   const Quaternion& Quaternion::operator *= (Quaternion& right) {

       //w *= right.w;
       //x *= right.x;
       //y *= right.y;
       //z *= right.z;

       w = (w * right.w) - (x * right.x) - (y * right.y) - (z * right.z);
       x = (w * right.x) + (x * right.w) + (y * right.z) - (z * right.y);
       y = (w * right.y) + (y * right.w) + (z * right.x) - (x * right.z);
       z = (w * right.z) + (z * right.w) + (x * right.y) - (y * right.x);

       return *this;
   }


   const Quaternion operator*(const Quaternion &left, const Quaternion &right) {

       Quaternion quat;

       quat.w = (left.w * right.w) - (left.x * right.x) - (left.y * right.y) - (left.z * right.z);
       quat.x = (left.w * right.x) + (left.x * right.w) + (left.y * right.z) - (left.z * right.y);
       quat.y = (left.w * right.y) + (left.y * right.w) + (left.z * right.x) - (left.x * right.z);
       quat.z = (left.w * right.z) + (left.z * right.w) + (left.x * right.y) - (left.y * right.x);

       return quat;

   }

   const Quaternion operator*(const Quaternion &q, const Cartesian_Vector &v) {

       return  Quaternion(  -(q.x*v.x + q.y*v.y + q.z*v.z),
                              q.w*v.x + q.y*v.z - q.z*v.y,
                              q.w*v.y + q.z*v.x - q.x*v.z,
                              q.w*v.z + q.x*v.y - q.y*v.x);
       }

   const Quaternion operator*(const Cartesian_Vector &v, const Quaternion &q) {

       return  Quaternion(  -(q.x*v.x + q.y*v.y + q.z*v.z),
                              q.w*v.x + q.z*v.y - q.y*v.z,
                              q.w*v.y + q.x*v.z - q.z*v.x,
                              q.w*v.z + q.y*v.x - q.x*v.y);
       }

    const Quaternion operator*(const Quaternion &q, const double &s)
    {
        return Quaternion(q.w * s, q.x * s, q.y * s, q.z * s);

    }
}
