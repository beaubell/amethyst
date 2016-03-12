#ifndef AMETHYST_CLIENT_SHADERPROG_UI_H
#define AMETHYST_CLIENT_SHADERPROG_UI_H

/***********************************************************************
 Amethyst-GL
  - ShaderProgramUI declarations

  This class is a specialization of ShaderProgram for the purpose of drawing UI elements (lines) on the screen.

 Authors (c):
 2016-2016 Beau V.C. Bellamy (beau@bellamy.beau@gmail.com)

 $Revision$
 $LastChangedDate$
 $LastChangedBy$
 ***********************************************************************/

#include "shaderprog.h"
#include "lib/types.h"

namespace amethyst {
namespace client{

class ShaderProgramUI : public ShaderProgram
{
  public:
    typedef std::shared_ptr<ShaderProgramUI> sptr;
    typedef std::weak_ptr<ShaderProgramUI>   wptr;

    ShaderProgramUI();

    void setProjM(const TransMatrix& m_proj);
    void setViewM(const TransMatrix& m_proj);

    void setColorV4(const Color& color);

    uint getVertexLoc();

  private:
    // Vertex Attribute Locations
    ShaderProgram::AttribHDL vertexLoc;

    // Uniform variable Locations
    ShaderProgram::UniformHDL projMatrixLoc;
    ShaderProgram::UniformHDL viewMatrixLoc;
    ShaderProgram::UniformHDL colorLoc;
};


} // namespace lib
} // namespace amethyst

#endif //AMETHYST_CLIENT_SHADERPROG_UI_H
