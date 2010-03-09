/***********************************************************************
 Amethyst-lib
  - Vector Class Objects Implementations

 Authors (c):
 2006-2008 Beau V.C. Bellamy (beau@stellarnetservices.net)

 $Revision$
 $LastChangedDate$
 $LastChangedBy$
 ***********************************************************************/

#include <math.h>
#include "vector.h"

namespace amethyst {
namespace lib {

  void Cartesian_Vector::clear(void) {

       x = 0;
       y = 0;
       z = 0;
       }

  double Cartesian_Vector::magnitude(void) const {

       return sqrt(x*x + y*y + z*z);
       }

  void Cartesian_Vector::normalize(void) {

       double m = magnitude();
       x /= m;
       y /= m;
       z /= m;
       }

  void Cartesian_Vector::reverse(void) {

       x = -x;
       y = -y;
       z = -z;
       }

    Cartesian_Vector& Cartesian_Vector::operator = (const Cartesian_Vector& vector)
    {
        x = vector.x;
        y = vector.y;
        z = vector.z;
        return *this;
    }

  const Cartesian_Vector& Cartesian_Vector::operator += (const Cartesian_Vector& vector) {

       x += vector.x;
       y += vector.y;
       z += vector.z;

       return *this;
       }

  const Cartesian_Vector& Cartesian_Vector::operator -= (const Cartesian_Vector& vector) {

       x -= vector.x;
       y -= vector.y;
       z -= vector.z;

       return *this;
       }

  const Cartesian_Vector& Cartesian_Vector::operator *= (const double& number) {

       x *= number;
       y *= number;
       z *= number;

       return *this;
       }

  const Cartesian_Vector& Cartesian_Vector::operator /= (const double& number) {

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

  bool operator == (const Cartesian_Vector &left, const Cartesian_Vector &right) {

      return ((left.x == right.x) && (left.y == right.y) && (left.z == right.z));
      }



      Spherical_Vector& Spherical_Vector::operator=(const Spherical_Vector &right)
      {
          a = right.a;
          p = right.p;
          r = right.r;

          return *this;
      }

  void Spherical_Vector::clear(void) {

       a = 0;
       p = 0;
       r = 0;
       }

} // namespace lib
} // namespace amethyst
