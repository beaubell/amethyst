/*
   Vector Classes, Definitions
   Amethyst Physics Library (c) 2003
   Author: Beau V.C. Bellamy
*/

// 
// 

#include <math.h>
#include "vector.h"

namespace amethyst {
       
  Cartesian_Vector::Cartesian_Vector(const Cartesian_Vector &old) {
  
       x = old.x;
       y = old.y;
       z = old.z;
       }
        
  void Cartesian_Vector::Zeroize(void) {
  
       x = 0;
       y = 0;
       z = 0;
       }

  double Cartesian_Vector::Magnitude(void) {

       return sqrt(x*x + y*y + z*z);
       }

  void Cartesian_Vector::Normalize(void) {

       double m = Magnitude();
       x /= m;
       y /= m;
       z /= m;
       }

  void Cartesian_Vector::Reverse(void) {

       x = -x;
       y = -y;
       z = -z;
       }
       
  //const Cartesian_Vector& Cartesian_Vector::operator = (Cartesian_Vector& vector) {
  
  //     x = vector.x;
  //     y = vector.y;
  //     z = vector.z;
       
  //     return *this;
  //     }
       
  const Cartesian_Vector& Cartesian_Vector::operator += (Cartesian_Vector& vector) {
  
       x += vector.x;
       y += vector.y;
       z += vector.z;
       
       return *this;
       }
       
  const Cartesian_Vector& Cartesian_Vector::operator -= (Cartesian_Vector& vector) {
  
       x -= vector.x;
       y -= vector.y;
       z -= vector.z;
       
       return *this;
       }
       
  const Cartesian_Vector& Cartesian_Vector::operator *= (double& number) {
  
       x *= number;
       y *= number;
       z *= number;
       
       return *this;
       }
       
  const Cartesian_Vector& Cartesian_Vector::operator /= (double& number) {
  
       x /= number;
       y /= number;
       z /= number;
       
       return *this;
       }

  Cartesian_Vector Cartesian_Vector::operator - (void) {

       return Cartesian_Vector(-x, -y, -z);
       }      
       
  const Cartesian_Vector operator+(const Cartesian_Vector &left, const Cartesian_Vector &right) {

       Cartesian_Vector vector;
         
       vector.x = left.x + right.x;
       vector.y = left.y + right.y;
       vector.z = left.z + right.z;

       return vector;
       }


  const Cartesian_Vector operator-(const Cartesian_Vector &left, const Cartesian_Vector &right) {

       Cartesian_Vector vector;
  
       vector.x = left.x - right.x;
       vector.y = left.y - right.y;
       vector.z = left.z - right.z;
       
       return vector;
       }

       
  const Cartesian_Vector operator*(const Cartesian_Vector &left, const double &multiplier) {
       
       Cartesian_Vector vector;
       
       vector.x = left.x * multiplier;
       vector.y = left.y * multiplier;
       vector.z = left.z * multiplier;
       
       return vector;
       }

       
  const Cartesian_Vector operator/(const Cartesian_Vector &left, const double &divisor) {
       
       Cartesian_Vector vector;
       
       vector.x = left.x / divisor;
       vector.y = left.y / divisor;
       vector.z = left.z / divisor;
       
       return vector;
       }

       
  //Spherical_Vector::Spherical_Vector(void){
  
  //     zeroize();
  //     }
       
  Spherical_Vector::Spherical_Vector(const Spherical_Vector &old) {
  
       a = old.a;
       p = old.p;
       r = old.r;
       }
        
  void Spherical_Vector::zeroize(void) {
  
       a = 0;
       p = 0;
       r = 0;
       }
       
}
