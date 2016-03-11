/***********************************************************************
 Amethyst-GL
  - ??? FIXME ??? implementations

 Authors (c):
 2016-2016 Beau V.C. Bellamy (beau@bellamy.beau@gmail.com)

 $Revision$
 $LastChangedDate$
 $LastChangedBy$
 ***********************************************************************/

#include "shaderprog_ui.h"

namespace amethyst {
namespace client{

ShaderProgramUI::ShaderProgramUI()
 : ShaderProgram("ui.vert","ui.frag"),
   vertexLoc(GetAttribLocation("vertexData")),
   projMatrixLoc(GetUniformLocation("projMatrix")),
   viewMatrixLoc(GetUniformLocation("viewMatrix")),
   colorLoc(GetUniformLocation("color"))
{
}

void ShaderProgramUI::setProjM(const TransMatrix& m_proj)
{
    UniformMatrix4f(projMatrixLoc, m_proj);
}

void ShaderProgramUI::setViewM(const TransMatrix& m_view)
{
    UniformMatrix4f(viewMatrixLoc, m_view);
}

void ShaderProgramUI::setColorV4(const Color& color)
{
    Uniform4f(colorLoc, color);
}

uint ShaderProgramUI::getVertexLoc()
{
    return vertexLoc.value;
}



} // namespace lib
} // namespace amethyst
