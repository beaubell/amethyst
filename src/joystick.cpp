/***************************************************************************
 *   Copyright (C) 2004 by Beau V.C. Bellamy                               *
 *   beau@borealisbroadband.net                                            *
 ***************************************************************************/
 
#include "joystick.h"

using namespace std;

short joystick_axis_norm(short value, unsigned short null)
{
  if (value > null) return (value - null);
  if (value < -null) return (value + null);
  return 0;
}
