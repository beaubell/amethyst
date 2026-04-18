#ifndef AMETHYST_CLIENT_JOYSTICK_H
#define AMETHYST_CLIENT_JOYSTICK_H

/***********************************************************************
 Amethyst-GL
  - Joystick input setup function declarations

 Authors (c):
 2004-2026 Beau V.C. Bellamy (bellamy.beau@gmail.com)
 ***********************************************************************/

#include <map>
#include "SDL3/SDL_joystick.h"

typedef struct {
   int joystick;
   int axis;
} Joy_Axis_Map;

short joystick_axis_norm(short value, unsigned short null);

namespace amethyst::client {


class Joystick
{
   public:
    Joystick();
    ~Joystick();

    unsigned short    joy_null;          // Null zone for joystick axeses
    short             joy_max;           // Max value for joystick axeses
    short             joy_min;

   private:
     std::map<SDL_JoystickID, SDL_Joystick*> joystick;      // Pointers to joysticks

};


} // namespace amethyst::client

#endif /* AMETHYST_CLIENT_JOYSTICK_H */
