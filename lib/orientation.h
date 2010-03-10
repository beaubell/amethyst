#ifndef AMETHYST_LIB_ORIENTATION_H
#define AMETHYST_LIB_ORIENTATION_H

/***********************************************************************
 Amethyst-lib
  - Orientation Class Objects Declarations

 Authors (c):
 2006-2008 Beau V.C. Bellamy (beau@stellarnetservices.net)

 $Revision$
 $LastChangedDate$
 $LastChangedBy$
 ***********************************************************************/

#include "vector.h"

namespace amethyst {
namespace lib {

  // Euler Data Type and Support Functions
  template <typename E>
  class Euler {

       public:

       E x;
       E y;
       E z;

       inline Euler() : x(0), y(0), z(0) {};
       inline Euler(const E &xx, const E &yy, const E &zz) : x(xx), y(yy), z(zz){};
       Euler(const Euler &right) : x(right.x), y(right.y), z(right.z) {};

       //void Set_Identity(void);

       //const Euler& operator = (Euler&);

       //const Euler& operator *= (Euler&);
       };

  //const Euler operator* (const Euler &left, const Euler &right);


  // Quaternion Data Type and Support Functions
  template <typename T>
  class Quaternion {

       public:

       T w;
       T x;
       T y;
       T z;

       inline Quaternion() : w(1), x(0), y(0), z(0) {};
       inline Quaternion(const T &ww, const T &xx, const T &yy, const T &zz) : w(ww), x(xx), y(yy), z(zz){};
       //Quaternion(const Quaternion<T>&);
       Quaternion(const Euler<T>& old)
       {
         Quaternion<T> Qx( cos(old.x/2.0), (sin(old.x/2.0)), 0, 0 );
         Quaternion<T> Qy( cos(old.y/2.0), 0, (sin(old.y/2.0)), 0 );
         Quaternion<T> Qz( cos(old.z/2.0), 0, 0, (sin(old.z/2.0)) );

         *this = Qz * Qy * Qx;
       }

       void normalize(void)
       {
         T l = length();

         x /= l;
         y /= l;
         z /= l;
         w /= l;
       }

       Cartesian_Vector<T> GetVector(void)
       {
         return Cartesian_Vector<T>(x,y,z);
       }
       
       
       Quaternion        Bar(void) { return Quaternion(w, -x, -y, -z); };
       Quaternion scale(const Scalar &s)
       {
         return Quaternion<T>(w * s, x * s, y * s, z * s);
       }
       
       T length(void)
       {
         return sqrt(x * x + y * y + z * z + w * w);
       }
       
       T dot(const Quaternion<T> &right) const
       {
         return (  w * right.w
                 + x * right.x
                 + y * right.y
                 + z * right.z );
       }
       
       void              slerp (const Quaternion &left, Quaternion &right, float t)
       {
         T w1, w2;
        
         T cos_theta = left.dot(right);
         T theta = acos(cos_theta);
         T sin_theta = sin(theta);

         if( sin_theta > 0.001 )
         {
           w1 = sin((1.0 - t)*theta) / sin_theta;
           w2 = sin(t * theta) / sin_theta;
         }
         else
         {
             w1 = 1.0 - t;
             w2 = t;
         }

         *this = left*w1 + right*w2;
       }
       
       void              setIdentity(void) {w=1; x=y=z=0; };
       bool              isIdentity() {return ( w==1 && x==0 && y==0 && z==0); };


       
       Quaternion operator*(const Scalar scale) const
       {
         return Quaternion(w * scale, x * scale, y * scale, z * scale);
       }
       
       Quaternion operator+(const Quaternion<T> &b) const
       {
         return Quaternion( w + b.w, x + b.x, y + b.y, z + b.z );
       }

       const Quaternion& operator *= (Quaternion<T>& right)
       {
         T tmp_w = (w * right.w) - (x * right.x) - (y * right.y) - (z * right.z);
         T tmp_x = (w * right.x) + (x * right.w) + (y * right.z) - (z * right.y);
         T tmp_y = (w * right.y) + (y * right.w) + (z * right.x) - (x * right.z);
         T tmp_z = (w * right.z) + (z * right.w) + (x * right.y) - (y * right.x);

         w = tmp_w;
         x = tmp_x;
         y = tmp_y;
         z = tmp_z;

         return *this;
       }
 
       const Quaternion<T>& operator *= (const Scalar& scale)
       {
         w = w * scale;
         x = x * scale;
         y = y * scale;
         z = z * scale;
         return *this;
       }

  };

  template <typename T>
  const Quaternion<T> operator* (const Quaternion<T> &left, const Quaternion<T> &right)
  {
    Quaternion<T> quat;

    quat.w = (left.w * right.w) - (left.x * right.x) - (left.y * right.y) - (left.z * right.z);
    quat.x = (left.w * right.x) + (left.x * right.w) + (left.y * right.z) - (left.z * right.y);
    quat.y = (left.w * right.y) + (left.y * right.w) + (left.z * right.x) - (left.x * right.z);
    quat.z = (left.w * right.z) + (left.z * right.w) + (left.x * right.y) - (left.y * right.x);

    return quat; 
  }
  
  template <typename T>
  const Quaternion<T> operator* (const Quaternion<T> &q, const Cartesian_Vector<T> &v)
  {
    return  Quaternion<T>(-(q.x*v.x + q.y*v.y + q.z*v.z),
                            q.w*v.x + q.y*v.z - q.z*v.y,
                            q.w*v.y + q.z*v.x - q.x*v.z,
                            q.w*v.z + q.x*v.y - q.y*v.x);
  }
  
  template <typename T>
  const Quaternion<T> operator* (const Cartesian_Vector<T> &v, const Quaternion<T> &q)
  {
    return  Quaternion<T>(-(q.x*v.x + q.y*v.y + q.z*v.z),
                            q.w*v.x + q.z*v.y - q.y*v.z,
                            q.w*v.y + q.x*v.z - q.z*v.x,
                            q.w*v.z + q.y*v.x - q.x*v.y);
  }
  //const Quaternion operator* (const Quaternion &q, const double &s);

  //template <typename T>
  //double     dot   (const Quaternion &left, const Quaternion &right);


  /// Rotate vector around a unit quaternion
  template <typename T>
  Cartesian_Vector<T> QVRotate(Quaternion<T> &q, const Cartesian_Vector<T> &v)
  {
    Quaternion<T> t;

    t = (q * v) * q.Bar();
    return t.GetVector();
  }
  
  
  // Specializations
  

} // namespace lib
} // namespace amethyst

#endif  /* AMETHYST_LIB_ORIENTATION_H */
