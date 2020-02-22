#ifndef AMETHYST_CLIENT_SHADERPROG_HUD_H
#define AMETHYST_CLIENT_SHADERPROG_HUD_H

/***********************************************************************
 Amethyst-GL
  - ShaderProgramUI declarations

  This class is a specialization of ShaderProgram for the purpose of drawing UI elements (lines) on the screen.

 Authors (c):
 2016-2020 Beau V.C. Bellamy (beau@bellamy.beau@gmail.com)
 ***********************************************************************/

#include "shaderprog.h"
#include "lib/types.h"

namespace amethyst {
namespace client{

class ShaderProgramHUD : public ShaderProgram
{
  public:
    typedef std::shared_ptr<ShaderProgramHUD> sptr;
    typedef std::weak_ptr<ShaderProgramHUD>   wptr;

    ShaderProgramHUD();

    void setProjM(const TransMatrix& m_proj);
    void setViewM(const TransMatrix& m_proj);
    
    void setFogColor(const Color& fogcolor);
    void setFogStart(const float fogstart);
    void setFogEnd(const float fogend);

    uint getVertexLoc();
    uint getVertexColorLoc();

  private:
    // Vertex Attribute Locations
    ShaderProgram::AttribHDL vertexLoc;
    ShaderProgram::AttribHDL vertexColLoc;

    // Uniform variable Locations
    ShaderProgram::UniformHDL projMatrixLoc;
    ShaderProgram::UniformHDL viewMatrixLoc;
    ShaderProgram::UniformHDL fogColorLoc;
    ShaderProgram::UniformHDL fogStartLoc;
    ShaderProgram::UniformHDL fogEndLoc;
};


} // namespace lib
} // namespace amethyst

#endif //AMETHYST_CLIENT_SHADERPROG_HUD_H
