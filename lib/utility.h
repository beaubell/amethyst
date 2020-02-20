/***********************************************************************
 Amethyst Physics Library
  -  Utility Functions, Declarations

 Authors (c):
 2003-2020 Beau V.C. Bellamy (beau@bellamy.beau@gmail.com)
 ***********************************************************************/

#ifndef UTILITY_H
#define UTILITY_H

#include "vector.h"
#include "object.h"
#include "universe.h"

#include <vector>
#include <iomanip>

namespace amethyst {
namespace lib {

  void print_vector(const std::string &title, const Cartesian_Vector &vector);
  void print_vector(const std::string &title, const Spherical_Vector &vector);
  void print_vector(const std::string &title, const Quaternion       &vector);
  void print_object(const Object &obj);

  void readTextFile(const std::string& filename, std::string& in_buffer);

  void gen_model_solarsystem(Universe &uni);
  void gen_object_variation(Universe &uni,
                            const Object &source,
                            const Cartesian_Vector &r_stepsize,
                            const Cartesian_Vector &v_stepsize,
                            const uint r_steps[3],
                            const uint v_steps[3],
                            const int  r_stepoff[3],
                            const int  v_stepoff[3]);

template <typename T>
std::string to_string(const T a_value, const int n = 6)
{
    std::ostringstream out;
    out << std::setprecision(n) << std::fixed << a_value;
    return out.str();
}


} // namespace lib
} // namespace amethyst

#endif /* UTILIty_H */
