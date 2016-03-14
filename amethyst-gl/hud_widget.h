#ifndef AMETHYST_CLIENT_HUD_WIDGET_H
#define AMETHYST_CLIENT_HUD_WIDGET_H

/***********************************************************************
 Amethyst-GL
  - ??? FIXME ??? declarations

 Authors (c):
 2016-2016 Beau V.C. Bellamy (beau@bellamy.beau@gmail.com)

 $Revision$
 $LastChangedDate$
 $LastChangedBy$
 ***********************************************************************/

#include "shaderprog_hud.h"

namespace amethyst {
namespace client {

class HUDRadial
{
  public:
    HUDRadial(ShaderProgramHUD::sptr uishader);
    virtual ~HUDRadial();
    void render();
    
  private:
    ShaderProgramHUD::sptr hudshader_;
    
    unsigned int _vao[1]; // Vertex Array Objects Identifier
    unsigned int _buffer[2]; // 0=lines, 1=colors

};


} // namespace lib
} // namespace amethyst

#endif //AMETHYST_CLIENT_HUD_WIDGET_H
