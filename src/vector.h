/*
   Vector Classes, Declarations
   Amethyst Physics Library (c) 2003
   Author: Beau V.C. Bellamy
*/

// 
// 

#ifndef AMETHYST_VECTOR_H
#define AMETHYST_VECTOR_H

namespace amethyst {

  // Cartesian_Vector Data Type and Support Functions
  class Cartesian_Vector {
     
     public:
       double x;
       double y;
       double z;
       
       inline Cartesian_Vector() : x(0), y(0), z(0) {};
       inline Cartesian_Vector(const double &xx, const double &yy, const double &zz) : x(xx), y(yy), z(zz){};
       Cartesian_Vector(const Cartesian_Vector&);

       void   Zeroize(void);

       double Magnitude(void);
       void   Normalize(void);
       void   Reverse(void);
       
       
       //const Cartesian_Vector& operator = (Cartesian_Vector&);
       
       const Cartesian_Vector& operator += (Cartesian_Vector&);
       const Cartesian_Vector& operator -= (Cartesian_Vector&);
       const Cartesian_Vector& operator *= (double&);
       const Cartesian_Vector& operator /= (double&);

       Cartesian_Vector operator -(void);
       };

  typedef Cartesian_Vector Cartesian_Coord;

  const Cartesian_Vector operator+ (const Cartesian_Vector &left, const Cartesian_Vector &right);
  const Cartesian_Vector operator- (const Cartesian_Vector &left, const Cartesian_Vector &right);
  const Cartesian_Vector operator* (const Cartesian_Vector &left, const double &scalar);
  const Cartesian_Vector operator/ (const Cartesian_Vector &left, const double &scalar);

    
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
       Spherical_Vector(const Spherical_Vector&);
       
       void zeroize(void);
              
       //const Spherical_Vector& operator = (Spherical_Vector&);
       
       const Spherical_Vector& operator += (Spherical_Vector&);
       const Spherical_Vector& operator -= (Spherical_Vector&);
       const Spherical_Vector& operator *= (double&);
       const Spherical_Vector& operator /= (double&);
       };

  typedef Spherical_Vector Spherical_Coord;
 
  const Spherical_Vector operator+ (const Spherical_Vector &left, const Spherical_Vector &right);
  const Spherical_Vector operator- (const Spherical_Vector &left, const Spherical_Vector &right);
  const Spherical_Vector operator* (const Spherical_Vector &left, const double &scalar);
  const Spherical_Vector operator/ (const Spherical_Vector &left, const double &scalar);

  //const Spherical_Vector& operator= (const Spherical_Vector &left, const Cartesian_Vector &right);
  //const Cartesian_Vector& operator= (const Cartesian_Vector &left, const Spherical_Vector &right);
  
}

#endif /* AMETHYST_VECTOR_H */
