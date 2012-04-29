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

typedef double (*root_func)(double x);


// Takes a function with one argument and returns the root within the bracket and tolerance.
double find_root(root_func fx, double tolerance, double start_point, double end_point);


} // namespace lib
} // namespace amethyst


#endif