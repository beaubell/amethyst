/***********************************************************************
 Amethyst-GL
  - ??? FIXME ??? implementations

 Authors (c):
 2016-2016 Beau V.C. Bellamy (beau@bellamy.beau@gmail.com)

 $Revision$
 $LastChangedDate$
 $LastChangedBy$
 ***********************************************************************/

#include "shaderprog_uifont.h"

namespace amethyst {
namespace client{

ShaderProgramFont::ShaderProgramFont()
 : ShaderProgram("uifont.vert","uifont.frag"),
   vertexLoc_(GetAttribLocation("vertexData")),
   texcoordLoc_(GetAttribLocation("texcoordData")),
   projMatrixLoc_(GetUniformLocation("projMatrix")),
   viewMatrixLoc_(GetUniformLocation("viewMatrix")),
   colorLoc_(GetUniformLocation("color")),
   texUnitLoc_(GetUniformLocation("baseTex"))
{
}

void ShaderProgramFont::setProjM(const TransMatrix& m_proj)
{
    UniformMatrix4f(projMatrixLoc_, m_proj);
}

void ShaderProgramFont::setViewM(const TransMatrix& m_view)
{
    UniformMatrix4f(viewMatrixLoc_, m_view);
}

void ShaderProgramFont::setColorV4(const Color& color)
{
    Uniform4f(colorLoc_, color);
}

void ShaderProgramFont::setTexUnit(uint unit)
{
    Uniform1i(texUnitLoc_, unit);
}

uint ShaderProgramFont::getVertexLoc()
{
    return vertexLoc_.value;
}

uint ShaderProgramFont::getTexCoordLoc()
{
    return texcoordLoc_.value;
}


} // namespace lib
} // namespace amethyst
