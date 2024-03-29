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

namespace amethyst::lib {

  void gen_model_solarsystem(Universe &uni);
  void gen_object_variation(Universe &uni,
                            const Object &source,
                            const Cartesian_Vector &r_stepsize,
                            const Cartesian_Vector &v_stepsize,
                            const unsigned int r_steps[3],
                            const unsigned int v_steps[3],
                            const int  r_stepoff[3],
                            const int  v_stepoff[3]);

template <typename T>
std::string to_string(const T a_value, const int n = 6)
{
    std::ostringstream out;
    out << std::setprecision(n) << std::fixed << a_value;
    return out.str();
}


} // namespace amethyst::lib

#endif /* UTILIty_H */
