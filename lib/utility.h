/*
   Utility Functions, Declarations
   Amethyst Physics Library (c) 2003
   Author: Beau V.C. Bellamy
*/

//
//

#ifndef UTILITY_H
#define UTILITY_H

#include "vector.h"
#include "object.h"
#include "universe.h"

#include <vector>

namespace amethyst {
namespace lib {

  void print_vector(const char *title, const Cartesian_Vector &vector);
  void print_vector(const char *title, const Spherical_Vector &vector);
  void print_vector(const char *title, const Quaternion       &vector);
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

} // namespace lib
} // namespace amethyst

#endif /* UTILIty_H */
