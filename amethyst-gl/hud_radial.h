#pragma once

/***********************************************************************
 Amethyst-GL
  - HUD Radial Class declarations

 Authors (c):
 2016-2016 Beau V.C. Bellamy (beau@bellamy.beau@gmail.com)
 ***********************************************************************/

#include "hud_widget.h"
#include "shaderprog_hud.h"
#include "bufferobj.h"
#include "scene.h" // for Eye::

namespace amethyst {
namespace client {

  
class HUDRadial : public HUDWidget
{
  public:
    HUDRadial(ShaderProgramHUD::sptr uishader);
    virtual ~HUDRadial();
    virtual void render(const Eye eye = Eye::MONO);
    virtual void update(){};
    
  private:
    ShaderProgramHUD::sptr hudshader_;
    
    VertexArray vao_; // Vertex Array Objects Identifier
    ArrayBuffer buffer_[2]; // 0=lines, 1=colors

};


} // namespace lib
} // namespace amethyst
