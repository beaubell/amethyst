#pragma once

/***********************************************************************
 Amethyst-GL
  - HUD Widget Base Class declarations

 Authors (c):
 2016-2016 Beau V.C. Bellamy (beau@bellamy.beau@gmail.com)
 ***********************************************************************/

namespace amethyst {
namespace client {

class HUDWidget
{
  public:
    //HUDWidget();
    virtual ~HUDWidget(){};
    virtual void render(const double eyeangle) = 0;
    virtual void update() = 0;
};
  

} // namespace lib
} // namespace amethyst

