/***********************************************************************
 Amethyst-GL
  - Joystick input setup function definitions

 Authors (c):
 2004-2008 Beau V.C. Bellamy (beau@stellarnetservices.net)

 $Revision$
 $LastChangedDate$
 $LastChangedBy$
 ***********************************************************************/

#include <iostream>

#include "SDL.h"

#include "joystick.h"
#include "global.h"

using namespace std;
using namespace amethyst;


short joystick_axis_norm(short value, unsigned short null)
{
  if (value > null) return (value - null);
  if (value < -null) return (value + null);
  return 0;
}

void joystick_setup()
{
    cout << "Initializing Joysticks...";

    SDL_InitSubSystem(SDL_INIT_JOYSTICK);

    cout << "OK!" << endl;

    // Check for joysticks
    Global.joysticks = SDL_NumJoysticks();
    cout << " Found " << Global.joysticks << " joysticks" << endl;

    for(int i = 0;i < Global.joysticks; i++){

      // Open joystick
      Global.joystick[i] = SDL_JoystickOpen(i);

      if(Global.joystick[i])
      {
        cout << " Opened Joystck " << i                                          << endl;
        cout << "  Name   : "      << SDL_JoystickName(i)                        << endl;
        cout << "  Axises : "      << SDL_JoystickNumAxes(Global.joystick[i])    << endl;
        cout << "  Buttons: "      << SDL_JoystickNumButtons(Global.joystick[i]) << endl;
        cout << "  Hats   : "      << SDL_JoystickNumHats(Global.joystick[i])    << endl;
        cout << "  Balls  : "      << SDL_JoystickNumBalls(Global.joystick[i])   << endl;
      }
      else
        cout << "NOES!1  Couldn't open Joystick " << i << endl;

    }

    if (Global.joysticks == 0)
      cout << "NOES!1 No Joysticks found." << endl;

}
