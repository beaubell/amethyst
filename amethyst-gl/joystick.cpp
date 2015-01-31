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

namespace amethyst {
namespace client {


Joystick::Joystick()
    : joy_null(5000),
      joy_max(32767),
      joy_min(-32768)
{
    cout << "Initializing Joysticks...";

    SDL_InitSubSystem(SDL_INIT_JOYSTICK);

    cout << "OK!" << endl;

    // Check for joysticks
    joysticks = SDL_NumJoysticks();
    cout << " Found " << joysticks << " joysticks" << endl;

    for(int i = 0;i < joysticks; i++){

        // Open joystick
        Global.joystick[i] = SDL_JoystickOpen(i);

        if(Global.joystick[i])
        {
            cout << " Opened Joystck " << i << endl;
            cout << "  Name   : " << SDL_JoystickName(Global.joystick[i]) << endl;
            cout << "  Axises : " << SDL_JoystickNumAxes(Global.joystick[i]) << endl;
            cout << "  Buttons: " << SDL_JoystickNumButtons(Global.joystick[i]) << endl;
            cout << "  Hats   : " << SDL_JoystickNumHats(Global.joystick[i])    << endl;
            cout << "  Balls  : " << SDL_JoystickNumBalls(Global.joystick[i])   << endl;
        }
        else
            cout << "NOES!1  Couldn't open Joystick " << i << endl;

    }

    if (joysticks == 0)
      cout << "NOES!1 No Joysticks found." << endl;
}


Joystick::~Joystick()
{
    /// FIXME This gets called after SDL_QUIT...
    /*
    for (int i = 0; i < joysticks; i++)
        if(Global.joystick[i])
            SDL_JoystickClose(Global.joystick[i]);

    SDL_QuitSubSystem(SDL_INIT_JOYSTICK);
    */
}

} // namespace client
} // namespace amethyst


short joystick_axis_norm(short value, unsigned short null)
{
  if (value > null) return (value - null);
  if (value < -null) return (value + null);
  return 0;
}
