/***********************************************************************
 Amethyst-lib
  - Root Finding Functions, Declaration

 Authors (c):
 2012-2012 Beau V.C. Bellamy (beau@bellamy.beau@gmail.com)

 $Revision:  $
 $LastChangedDate: $
 $LastChangedBy: $
 ***********************************************************************/

#include "root.h"
#include <cmath>


namespace amethyst {
namespace lib{


// Adapted from my bisection routine
float_type find_root(root_func fx, float_type tolerance, float_type left, float_type right)
{

  uint iterations = 0;

  //% Calculate the expected number of iterations (and do nothing with it except return it)
  //uint expect_iters = (log(right-left) - log(tolerance))/log(2);

  while(1)
  {
      float_type middle = ( left+right ) / 2.0 ;

      if ( abs ( ( right-left ) / middle ) < tolerance)
        return middle;


      float_type fLeft = fx( left ) ;
      float_type fMiddle = fx( middle ) ;

      if ( fLeft * fMiddle <= 0.0 )
        right = middle;
      else
        left = middle;


      iterations = iterations + 1 ;
  }



}

  
} // namespace lib
} // namespace amethyst