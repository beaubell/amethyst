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

#include <vector>

namespace amethyst {
namespace lib {

  void print_vector(const char *title, const Cartesian_Vector &vector);
  void print_vector(const char *title, const Spherical_Vector &vector);
  void print_vector(const char *title, const Quaternion       &vector);
  void print_object(const Object &obj);

  void readTextFile(const std::string& filename, std::string& in_buffer);

  void dumpVectorHDF5(const std::string &filename, const std::vector<Cartesian_Vector> &vec_in);

} // namespace lib
} // namespace amethyst

#endif /* UTILIty_H */
