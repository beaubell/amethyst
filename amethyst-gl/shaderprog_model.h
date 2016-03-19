#ifndef AMETHYST_CLIENT_SHADERPROG_MODEL_H
#define AMETHYST_CLIENT_SHADERPROG_MODEL_H

/***********************************************************************
 Amethyst-GL
  - ??? FIXME ??? declarations

 Authors (c):
 2016-2016 Beau V.C. Bellamy (beau@bellamy.beau@gmail.com)

 $Revision$
 $LastChangedDate$
 $LastChangedBy$
 ***********************************************************************/

#include "shaderprog.h"
#include "lib/types.h"

namespace amethyst {
namespace client {

class LightInfo 
{
public:
    glm::vec4 Position;
    glm::vec3 Intensity;
};

class MaterialInfo {
public:
    glm::vec3 Ka;
    glm::vec3 Kd;
    glm::vec3 Ks;
    float Shininess;
};
  
class ShaderProgramModel : public ShaderProgram
{
  public:
    typedef std::shared_ptr<ShaderProgramModel> sptr;
    typedef std::weak_ptr<ShaderProgramModel>   wptr;

    ShaderProgramModel();

    void setProjM(const TransMatrix& m_proj);
    void setModelViewM(const TransMatrix& m_modelview);
    void setNormalM(const NormalMatrix& m_normal);
    void setMVP(const TransMatrix& m_mvp);
    
    void setLight(const LightInfo& s_light);
    void setMaterial(const MaterialInfo& s_material);

    void setTexUnit(uint unit);

    uint getVertexLoc();
    uint getTexCoordLoc();
    uint getNormalLoc();

  private:
    // Vertex Attribute Locations
    ShaderProgram::AttribHDL vertexLoc_;
    ShaderProgram::AttribHDL texcoordLoc_;
    ShaderProgram::AttribHDL normalLoc_;

    // Uniform variable Locations
    ShaderProgram::UniformHDL projMatrixLoc_;
    ShaderProgram::UniformHDL modelViewMatrixLoc_;
    ShaderProgram::UniformHDL normalMatrixLoc_;
    ShaderProgram::UniformHDL mvpMatrixLoc_;
    ShaderProgram::UniformHDL texUnitLoc_;
    ShaderProgram::UniformHDL lightPosLoc_;
    ShaderProgram::UniformHDL lightIntenLoc_;
    ShaderProgram::UniformHDL MaterialKaLoc_;
    ShaderProgram::UniformHDL MaterialKdLoc_;
    ShaderProgram::UniformHDL MaterialKsLoc_;
    ShaderProgram::UniformHDL MaterialShineLoc_;
};


} // namespace lib
} // namespace amethyst

#endif //AMETHYST_CLIENT_SHADERPROG_MODEL_H
