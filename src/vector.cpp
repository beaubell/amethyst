/*
   Vector Classes, Definitions
   Amethyst Physics Library (c) 2003
   Author: Beau V.C. Bellamy
*/

// 
// 

#include "vector.h"

#ifdef __GNUG__
#pragma implementation
#endif

namespace amethyst {

  //Cartesian_Vector::Cartesian_Vector(void){
  
  //     zeroize();
  //     }
       
  Cartesian_Vector::Cartesian_Vector(const Cartesian_Vector &old) {
  
       x = old.x;
       y = old.y;
       z = old.z;
       }
        
  void Cartesian_Vector::zeroize(void) {
  
       x = 0;
       y = 0;
       z = 0;
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
