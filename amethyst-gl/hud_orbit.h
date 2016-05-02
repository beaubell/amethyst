#pragma once

/***********************************************************************
 Amethyst-GL
  - HUD Orbit Class declarations

 Authors (c):
 2016-2016 Beau V.C. Bellamy (beau@bellamy.beau@gmail.com)
 ***********************************************************************/

#include "hud_widget.h"
#include "shaderprog_hud.h"
#include "bufferobj.h"

namespace amethyst {
namespace client {

  
class HUDOrbit : public HUDWidget
{
  public:
    HUDOrbit(ShaderProgramHUD::sptr uishader);
    virtual ~HUDOrbit();
    virtual void render(const Eye eye = Eye::MONO);
    virtual void update(){};
    
  private:
    ShaderProgramHUD::sptr hudshader_;
    
    VertexArray vao_[2]; // Vertex Array Objects Identifier
    ArrayBuffer buffer_[4]; // 0=lines, 1=colors
    
    unsigned int orbitpoints_;

};


} // namespace lib
} // namespace amethyst
