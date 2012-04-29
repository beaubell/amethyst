#ifndef AMETHYST_CLIENT_SIMSTATS_H
#define AMETHYST_CLIENT_SIMSTATS_H

/***********************************************************************
 Amethyst-GL
  - ??? FIXME ??? declarations

 Authors (c):
 2012-2012 Beau V.C. Bellamy (beau@stellarnetservices.net)

 $Revision$
 $LastChangedDate$
 $LastChangedBy$
 ***********************************************************************/

#include "../amethyst-gl.h"

namespace amethyst {
namespace client {

class UIW_SimStats : public UI_Window
{
  public:
    UIW_SimStats(Amethyst_GL &amgl, UI &ui);
    void render();

  private:
    Amethyst_GL &_amgl;

};


} // namespace lib
} // namespace amethyst

#endif //AMETHYST_LIB_SIMSTATS_H
