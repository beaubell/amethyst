#ifndef AMETHYST_CLIENT_SHADERPROG_FONT_H
#define AMETHYST_CLIENT_SHADERPROG_FONT_H

/***********************************************************************
 Amethyst-GL
 - ShaderProgramFont declaration

  This class is a specialization of ShaderProgram for the purpose of drawing UI Text elements on the screen.

 Authors (c):
 2016-2020 Beau V.C. Bellamy (beau@bellamy.beau@gmail.com)
 ***********************************************************************/

#include "shaderprog.h"
#include "lib/types.h"

namespace amethyst {
namespace client {

class ShaderProgramFont : public ShaderProgram
{
  public:
    typedef std::shared_ptr<ShaderProgramFont> sptr;
    typedef std::weak_ptr<ShaderProgramFont>   wptr;

    ShaderProgramFont();

    void setProjM(const TransMatrix& m_proj);
    void setViewM(const TransMatrix& m_proj);

    void setColorV4(const Color& color);
    void setTexUnit(unsigned int  unit);

    unsigned int  getVertexLoc();
    unsigned int  getTexCoordLoc();

  private:
    // Vertex Attribute Locations
    ShaderProgram::AttribHDL vertexLoc_;
    ShaderProgram::AttribHDL texcoordLoc_;

    // Uniform variable Locations
    ShaderProgram::UniformHDL projMatrixLoc_;
    ShaderProgram::UniformHDL viewMatrixLoc_;
    ShaderProgram::UniformHDL colorLoc_;
    ShaderProgram::UniformHDL texUnitLoc_;
};


} // namespace lib
} // namespace amethyst

#endif //AMETHYST_CLIENT_SHADERPROG_FONT_H
