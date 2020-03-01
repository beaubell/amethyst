#pragma once

/***********************************************************************
 Amethyst-GL
  - HUD Widget Base Class declarations

 Authors (c):
 2016-2020 Beau V.C. Bellamy (bellamy.beau@gmail.com)
 ***********************************************************************/

#include "camera.h"

namespace amethyst {
namespace client {

class HUDWidget
{
  public:
    //HUDWidget();
    virtual ~HUDWidget(){};
    virtual void render(const Camera& camera, const Eye eye = Eye::MONO) = 0;
    virtual void update() = 0;
};
  

} // namespace lib
} // namespace amethyst
