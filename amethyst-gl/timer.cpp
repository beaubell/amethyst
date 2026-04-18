/***********************************************************************
 Amethyst-GL
  - Timing function implementations

 Authors (c):
 2006-2026 Beau V.C. Bellamy (bellamy.beau@gmail.com)
 ***********************************************************************/


#include "timer.h"
#include "global.h"
#include "SDL3/SDL_timer.h"

namespace amethyst::client {

unsigned int time_left(void)
{
  unsigned int now;

  now = SDL_GetTicks();
  if(Global.next_time <= now)
    return 0;
  else
    return Global.next_time - now;
}

} // namespace amethyst::client
