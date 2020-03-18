/***********************************************************************
 Amethyst-GL
 - ShaderProgramFont implementation

 Authors (c):
 2016-2020 Beau V.C. Bellamy (beau@bellamy.beau@gmail.com)
 ***********************************************************************/

#include "shaderprog_uifont.h"

#include "global.h"

DECLARE_RESOURCE(amethyst_gl_shaders_uifont_vert);
DECLARE_RESOURCE(amethyst_gl_shaders_uifont_frag);

namespace amethyst {
namespace client{

ShaderProgramFont::ShaderProgramFont()
:  ShaderProgram(LOAD_RESOURCE(amethyst_gl_shaders_uifont_vert, Global.dir_shaders, "uifont.vert"),
                 LOAD_RESOURCE(amethyst_gl_shaders_uifont_frag, Global.dir_shaders, "uifont.frag")),
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

void ShaderProgramFont::setTexUnit(unsigned int  unit)
{
    Uniform1i(texUnitLoc_, unit);
}

unsigned int  ShaderProgramFont::getVertexLoc()
{
    return vertexLoc_.value;
}

unsigned int  ShaderProgramFont::getTexCoordLoc()
{
    return texcoordLoc_.value;
}


} // namespace lib
} // namespace amethyst
