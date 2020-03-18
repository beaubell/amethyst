/***********************************************************************
 Amethyst-GL
 - ShaderProgramUI implementation

 Authors (c):
 2016-2020 Beau V.C. Bellamy (beau@bellamy.beau@gmail.com)
 ***********************************************************************/

#include "shaderprog_ui.h"

#include "global.h"

DECLARE_RESOURCE(amethyst_gl_shaders_ui_vert);
DECLARE_RESOURCE(amethyst_gl_shaders_ui_frag);

namespace amethyst {
namespace client{

ShaderProgramUI::ShaderProgramUI()
:  ShaderProgram(LOAD_RESOURCE(amethyst_gl_shaders_ui_vert, Global.dir_shaders, "ui.vert"),
                 LOAD_RESOURCE(amethyst_gl_shaders_ui_frag, Global.dir_shaders, "ui.frag")),
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

unsigned int  ShaderProgramUI::getVertexLoc()
{
    return vertexLoc.value;
}



} // namespace lib
} // namespace amethyst
