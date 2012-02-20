/***********************************************************************
 Amethyst-GL
  - Timing function implementations

 Authors (c):
 2006-2008 Beau V.C. Bellamy (beau@stellarnetservices.net)

 $Revision$
 $LastChangedDate$
 $LastChangedBy$
 ***********************************************************************/


#include "timer.h"
#include "global.h"

namespace amethyst {
namespace client {

unsigned int time_left(void)
{
  unsigned int now;

  now = SDL_GetTicks();
  if(Global.next_time <= now)
    return 0;
  else
    return Global.next_time - now;
}

} // namespace client
} // namespace amethyst
