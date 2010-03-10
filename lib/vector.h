#ifndef AMETHYST_LIB_VECTOR_H
#define AMETHYST_LIB_VECTOR_H

/***********************************************************************
 Amethyst-lib
  - Vector Class Objects Declarations

 Authors (c):
 2006-2008 Beau V.C. Bellamy (beau@stellarnetservices.net)

 $Revision$
 $LastChangedDate$
 $LastChangedBy$
 ***********************************************************************/

#include "datatypes.h"

namespace amethyst {
namespace lib {

  // Cartesian_Vector Data Type and Support Functions
  template <typename T>
  class Cartesian_Vector {

     public:
       T x;
       T y;
       T z;

       Cartesian_Vector() : x(0), y(0), z(0) {};
       Cartesian_Vector(const T &xx, const T &yy, const T &zz) : x(xx), y(yy), z(zz){};
       Cartesian_Vector(const Cartesian_Vector &right) : x(right.x), y(right.y), z(right.z){};
       virtual ~Cartesian_Vector(void) {};

       void clear(void)
       {
         x = 0;
         y = 0;
         z = 0;
       }

       T magnitude(void) const
       {
         return sqrt(x*x + y*y + z*z);
       }

       void normalize(void)
       {
         T m = magnitude();
         x /= m;
         y /= m;
         z /= m;
       }
       
       void reverse(void)
       {
         x = -x;
         y = -y;
         z = -z;
       }

       const Cartesian_Vector& operator += (const Cartesian_Vector<T>& left)
       {
	 x += left.x;
	 y += left.y;
	 z += left.z;
	 return *this;
       }
       
       const Cartesian_Vector& operator -= (const Cartesian_Vector<T>& left)
       {
	 x -= left.x;
	 y -= left.y;
	 z -= left.z; 
	 return *this;
       }
       const Cartesian_Vector& operator *= (const Scalar& number)
       {
         x *= number;
         y *= number;
         z *= number;
         return *this;
       }
       
       const Cartesian_Vector& operator /= (const Scalar& number)
       {
	 x /= number;
         y /= number;
         z /= number;
         return *this;
       }

       //Cartesian_Vector operator -(void);
       //Cartesian_Vector& operator =(const Cartesian_Vector &right);
  };

  //typedef Cartesian_Vector Cartesian_Coord;
  template <typename T>
  const Cartesian_Vector<T> operator+ (const Cartesian_Vector<T> &left, const Cartesian_Vector<T> &right)
  {
    return Cartesian_Vector<T>(left.x + right.x, left.y + right.y, left.z + right.z);
  }
  
  template <typename T>
  const Cartesian_Vector<T> operator- (const Cartesian_Vector<T> &left, const Cartesian_Vector<T> &right)
  {
    return Cartesian_Vector<T>(left.x - right.x, left.y - right.y, left.z - right.z);
  }
  
  template <typename RetVec, typename Vec, typename Scal>
  const Cartesian_Vector<RetVec> operator* (const Cartesian_Vector<Vec> &left, const Scal &scalar)
  {
    return Cartesian_Vector<RetVec>(left.x * scalar, left.y * scalar, left.z * scalar);
  }
  
  template <typename RetVec, typename Vec, typename Scal>
  const Cartesian_Vector<RetVec> operator/ (const Cartesian_Vector<Vec> &left, const Scal &scalar)
  {
    return Cartesian_Vector<RetVec>(left.x / scalar, left.y / scalar, left.z / scalar);
  }

  /// XTREME FIXME FIXME FIXME FIXME FIXME
  inline const Cartesian_Vector<Acceleration> operator/ (const Cartesian_Vector<Force> &left, const Mass &scalar)
  {
    return Cartesian_Vector<Acceleration>(left.x / scalar, left.y / scalar, left.z / scalar);
  }

  inline const Cartesian_Vector<Velocity> operator* (const Cartesian_Vector<Acceleration> &left, const Time &scalar)
  {
    return Cartesian_Vector<Velocity>(left.x * scalar, left.y * scalar, left.z * scalar);
  }
  
  inline const Cartesian_Vector<Location> operator* (const Cartesian_Vector<Velocity> &left, const Time &scalar)
  {
    return Cartesian_Vector<Location>(left.x * scalar, left.y * scalar, left.z * scalar);
  }
  //bool  operator == (const Cartesian_Vector &left, const Cartesian_Vector &right);


  // Spherical_Vector Data Type and Support Functions
  template <typename T>
  class Spherical_Vector {

       public:
       // Azimuthal angle in the xy plane from the z axis
       // radians, 0 <= a < 2PI
       T a;

       // Polar angle from the z axis
       // radians, 0 <= p <= PI
       T p;

       // radius, distance from the center
       T r;

       inline Spherical_Vector() : a(0), p(0), r(0) {};
       inline Spherical_Vector(const T &aa, const T &pp, const T &rr) : a(aa), p(pp), r(rr) {};
       Spherical_Vector(const Spherical_Vector &right) : a(right.a), p(right.p), r(right.r) {};
       virtual ~Spherical_Vector(void) {};

       void  clear(void)
       {
         a = 0;
         p = 0;
         r = 0;
       }

//       Spherical_Vector& operator = (const Spherical_Vector &right);

//       const Spherical_Vector& operator += (Spherical_Vector&);
//       const Spherical_Vector& operator -= (Spherical_Vector&);
//       const Spherical_Vector& operator *= (double&);
//       const Spherical_Vector& operator /= (double&);
  };

//  typedef Spherical_Vector Spherical_Coord;

//  const Spherical_Vector operator+ (const Spherical_Vector &left, const Spherical_Vector &right);
//  const Spherical_Vector operator- (const Spherical_Vector &left, const Spherical_Vector &right);
//  const Spherical_Vector operator* (const Spherical_Vector &left, const double &scalar);
//  const Spherical_Vector operator/ (const Spherical_Vector &left, const double &scalar);

  //const Spherical_Vector& operator= (const Spherical_Vector &left, const Cartesian_Vector &right);
  //const Cartesian_Vector& operator= (const Cartesian_Vector &left, const Spherical_Vector &right);

} // namespace lib
} // namespace amethyst

#endif /* AMETHYST_LIB_VECTOR_H */
