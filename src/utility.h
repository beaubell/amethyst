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

#ifdef __GNUG__
#pragma interface
#endif

namespace amethyst {

  void print_vector(char *title, const Cartesian_Vector &vector);
  void print_vector(char *title, const Spherical_Vector &vector);
  void print_vector(char *title, const Quaternion       &vector);
  void print_object(const Object &obj);

}

#endif /* UTILIty_H */
