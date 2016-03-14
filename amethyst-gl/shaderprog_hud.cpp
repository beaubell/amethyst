/***********************************************************************
 Amethyst-GL
  - ??? FIXME ??? implementations

 Authors (c):
 2016-2016 Beau V.C. Bellamy (beau@bellamy.beau@gmail.com)

 $Revision$
 $LastChangedDate$
 $LastChangedBy$
 ***********************************************************************/

#include "shaderprog_hud.h"

namespace amethyst {
namespace client{

ShaderProgramHUD::ShaderProgramHUD()
 : ShaderProgram("hud.vert","hud.frag"),
   vertexLoc(GetAttribLocation("vertexLocation")),
   vertexColLoc(GetAttribLocation("vertexColor")),
   projMatrixLoc(GetUniformLocation("projMatrix")),
   viewMatrixLoc(GetUniformLocation("viewMatrix")),
   fogColorLoc(GetUniformLocation("fogColor")),
   fogStartLoc(GetUniformLocation("fogStart")),
   fogEndLoc(GetUniformLocation("fogEnd"))
{
}

void ShaderProgramHUD::setProjM(const TransMatrix& m_proj)
{
    UniformMatrix4f(projMatrixLoc, m_proj);
}

void ShaderProgramHUD::setViewM(const TransMatrix& m_view)
{
    UniformMatrix4f(viewMatrixLoc, m_view);
}

void ShaderProgramHUD::setFogColor(const Color& fogcolor)
{
    Uniform4f(fogColorLoc, fogcolor);
}

void ShaderProgramHUD::setFogStart(const float fogstart)
{
    Uniform1f(fogStartLoc, fogstart);
}

void ShaderProgramHUD::setFogEnd(const float fogend)
{
    Uniform1f(fogEndLoc, fogend);
}


uint ShaderProgramHUD::getVertexLoc()
{
    return vertexLoc.value;
}

uint ShaderProgramHUD::getVertexColorLoc()
{
    return vertexColLoc.value;
}

} // namespace lib
} // namespace amethyst
