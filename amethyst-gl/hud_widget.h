#pragma once

/***********************************************************************
 Amethyst-GL
  - HUD Widget Base Class declarations

 Authors (c):
 2016-2016 Beau V.C. Bellamy (beau@bellamy.beau@gmail.com)
 ***********************************************************************/

#include "camera.h"

namespace amethyst {
namespace client {

class HUDWidget
{
  public:
    //HUDWidget();
    virtual ~HUDWidget(){};
    virtual void render(const Eye eye = Eye::MONO) = 0;
    virtual void update() = 0;
};
  

} // namespace lib
} // namespace amethyst

