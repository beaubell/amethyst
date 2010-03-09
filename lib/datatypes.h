#ifndef AMETHYST_LIB_DATATYPES_H
#define AMETHYST_LIB_DATATYPES_H

/***********************************************************************
 Amethyst-lib
  - Datatypes and Unit Declarations

 Authors (c):
 2010-2010 Beau V.C. Bellamy (beau@stellarnetservices.net)

 $Revision$
 $LastChangedDate$
 $LastChangedBy$
 ***********************************************************************/


#include <boost/units/systems/si.hpp> // Dimensional Analysis

#include <boost/units/systems/si/io.hpp> // Testing

namespace amethyst {
namespace lib {

#ifdef UNITS_FLOAT
typedef float Float_Type;
#else
typedef double Float_Type;
#endif

using namespace boost::units;
using namespace boost::units::si;

typedef quantity<si::length, Float_Type> Location;
typedef quantity<si::velocity, Float_Type> Velocity;
typedef quantity<si::acceleration, Float_Type> Acceleration;
typedef quantity<si::time, Float_Type> Time;

} // namespace lib
} // namespace amethyst

#endif
