/***********************************************************************
 Amethyst-lib
  - Vector Class Objects Implementations

 Authors (c):
 2006-2023 Beau V.C. Bellamy (bellamy.beau@gmail.com)
 ***********************************************************************/

#include "vector.h"

#include <cmath>

#include "types.h"
#include "yaml-cpp/yaml.h"

namespace amethyst::lib {

  void Cartesian_Vector::clear(void) {

       x = 0;
       y = 0;
       z = 0;
       }

  double Cartesian_Vector::magnitude(void) const {

       return sqrt(x*x + y*y + z*z);
       }

  void Cartesian_Vector::normalize(void) {

       const double m = magnitude();
       x /= m;
       y /= m;
       z /= m;
       }

  void Cartesian_Vector::reverse(void) {

       x = -x;
       y = -y;
       z = -z;
       }

YAML::Node
Cartesian_Vector::toYAML() const
{
    using namespace YAML;

    Node vector;
    vector.SetStyle(EmitterStyle::Flow);
    vector["x"] = x;
    vector["y"] = y;
    vector["z"] = z;

    return vector;
}

void
Cartesian_Vector::fromYAML(const YAML::Node& vec){

    using namespace YAML;

    x = vec["x"].as<float_type>();
    y = vec["y"].as<float_type>();
    z = vec["z"].as<float_type>();
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

  Cartesian_Vector Cartesian_Vector::operator-() const {

       return {-x, -y, -z};
       }

  Cartesian_Vector operator+(const Cartesian_Vector &left, const Cartesian_Vector &right) {

       Cartesian_Vector vector;

       vector.x = left.x + right.x;
       vector.y = left.y + right.y;
       vector.z = left.z + right.z;

       return vector;
       }


  Cartesian_Vector operator-(const Cartesian_Vector &left, const Cartesian_Vector &right) {

       Cartesian_Vector vector;

       vector.x = left.x - right.x;
       vector.y = left.y - right.y;
       vector.z = left.z - right.z;

       return vector;
       }


  Cartesian_Vector operator*(const Cartesian_Vector &left, const double &multiplier) {

       Cartesian_Vector vector;

       vector.x = left.x * multiplier;
       vector.y = left.y * multiplier;
       vector.z = left.z * multiplier;

       return vector;
       }


  Cartesian_Vector operator/(const Cartesian_Vector &left, const double &divisor) {

       Cartesian_Vector vector;

       vector.x = left.x / divisor;
       vector.y = left.y / divisor;
       vector.z = left.z / divisor;

       return vector;
       }

  bool operator == (const Cartesian_Vector &left, const Cartesian_Vector &right) {

      return ((left.x == right.x) && (left.y == right.y) && (left.z == right.z));
      }

std::ostream&
operator<<(std::ostream& os, const Cartesian_Vector& cv) {

    os << "CV[ x:"
       << cv.x << ", y:"
       << cv.y << ", z:"
       << cv.z
       << " ]";

    return os;
}


  void Spherical_Vector::clear(void) {

       a = 0;
       p = 0;
       r = 0;
       }

YAML::Node
Spherical_Vector::toYAML() const
{
    using namespace YAML;

    Node vector;
    vector.SetStyle(EmitterStyle::Flow);
    vector["a"] = a;
    vector["p"] = p;
    vector["r"] = r;

    return vector;
}

void
Spherical_Vector::fromYAML(const YAML::Node& vec){

    using namespace YAML;

    a = vec["a"].as<float_type>();
    p = vec["p"].as<float_type>();
    r = vec["r"].as<float_type>();
}

std::ostream&
operator<<(std::ostream& os, const Spherical_Vector& sv) {

    os << "SV[ a:"
    << sv.a << ", p:"
    << sv.p << ", r:"
    << sv.r
    << " ]";

    return os;
}

} // namespace amethyst::lib
