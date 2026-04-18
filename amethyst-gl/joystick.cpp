/***********************************************************************
 Amethyst-GL
  - Joystick input setup function definitions

 Authors (c):
 2004-2026 Beau V.C. Bellamy (bellamy.beau@gmail.com)
 ***********************************************************************/

#include <iostream>

#include "SDL3/SDL_joystick.h"
#include "SDL3/SDL.h"

#include "joystick.h"
#include "global.h"

using namespace std;
using namespace amethyst;

namespace amethyst::client {


Joystick::Joystick()
    : joy_null(5000),
      joy_max(32767),
      joy_min(-32768)
{
    cout << "Initializing Joysticks...";

    SDL_Init(SDL_INIT_JOYSTICK);

    cout << "OK!" << endl;

    // Check for joysticks
    int numJoysticks = 0;
    SDL_JoystickID* joysticksIDs = SDL_GetJoysticks(&numJoysticks);
    cout << " Found " << numJoysticks << " joysticks" << endl;

    for(int i = 0;i < numJoysticks; i++){

        auto joyId = joysticksIDs[i];
        // Open joystick
        joystick[joyId] = SDL_OpenJoystick(i);

        if(joystick[i])
        {
            cout << " Opened Joystck " << i << endl;
            cout << "  Name   : " << SDL_GetJoystickName(joystick[joyId]) << endl;
            cout << "  Axises : " << SDL_GetNumJoystickAxes(joystick[joyId]) << endl;
            cout << "  Buttons: " << SDL_GetNumJoystickButtons(joystick[joyId]) << endl;
            cout << "  Hats   : " << SDL_GetNumJoystickHats(joystick[joyId])    << endl;
            cout << "  Balls  : " << SDL_GetNumJoystickBalls(joystick[joyId])   << endl;
        }
        else
            cout << "NOES!1  Couldn't open Joystick " << i << endl;

    }

    if (numJoysticks == 0)
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

} // namespace amethyst::client


short joystick_axis_norm(short value, unsigned short null)
{
  if (value > null) return (value - null);
  if (value < -null) return (value + null);
  return 0;
}
