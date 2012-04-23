#ifndef AMETHYST_LIB_ROOT_H
#define AMETHYST_LIB_ROOT_H

/***********************************************************************
 Amethyst-lib
  - Root Finding Functions, Declaration

 Authors (c):
 2012-2012 Beau V.C. Bellamy (beau@bellamy.beau@gmail.com)

 $Revision:  $
 $LastChangedDate: $
 $LastChangedBy: $
 ***********************************************************************/

#include "types.h"

namespace amethyst {
namespace lib{

typedef float_type (*root_func)(float_type x);


// Takes a function with one argument and returns the root within the bracket and tolerance.
float_type find_root(root_func fx, float_type tolerance, float_type start_point, float_type end_point);


} // namespace lib
} // namespace amethyst


#endif