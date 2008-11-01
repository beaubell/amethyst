#ifndef JOYSTICK_H
#define JOYSTICK_H

/***********************************************************************
 Amethyst-GL
  - Joystick input setup function declarations

 Authors (c):
 2004-2008 Beau V.C. Bellamy (beau@stellarnetservices.net)

 $Revision$
 $LastChangedDate$
 $LastChangedBy$
 ***********************************************************************/

#include "SDL.h"

typedef struct {
   int joystick;
   int axis;
} Joy_Axis_Map;

short joystick_axis_norm(short value, unsigned short null);

namespace amethyst {
namespace client {


class Joystick
{
   public:
    Joystick();
    ~Joystick();

    unsigned short    joy_null;          // Null zone for joystick axeses
    short             joy_max;           // Max value for joystick axeses
    short             joy_min;

   private:
     int               joysticks;         // Number of joysticks
     SDL_Joystick     *joystick[16];      // Pointers to joysticks

};


} // namespace client
} // namespace amethyst

#endif /* JOYSTICK_H */
