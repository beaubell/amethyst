/***********************************************************************
 Amethyst-GL
 - ShaderProgramModel implementation

 Authors (c):
 2016-2020 Beau V.C. Bellamy (beau@bellamy.beau@gmail.com)
 ***********************************************************************/

#include "shaderprog_model.h"
#include "global.h"

#include "yaml-cpp/yaml.h"

#include <iostream>

DECLARE_RESOURCE(amethyst_gl_shaders_model_vert);
DECLARE_RESOURCE(amethyst_gl_shaders_model_frag);

namespace amethyst {
namespace client{

MaterialInfo::MaterialInfo(YAML::Node ymatinfo) {
    using namespace YAML;

    Node yka = ymatinfo["ambient"];
    Node ykd = ymatinfo["diffuse"];
    Node yks = ymatinfo["specular"];
    Node yshininess = ymatinfo["shininess"];

    if (yka.IsSequence() && yka.size() == 3) {
        for (int i = 0; i < yka.size(); i++)
            Ka[i] = yka[i].as<float_type>();
        std::cout << Ka[0] << Ka[1] << Ka[2] << std::endl;
    }

    if (ykd.IsSequence() && ykd.size() == 3) {
        for (int i = 0; i < ykd.size(); i++)
            Kd[i] = ykd[i].as<float_type>();
    }

    if (yks.IsSequence() && yks.size() == 3) {
        for (int i = 0; i < yks.size(); i++)
            Ks[i] = yks[i].as<float_type>();
    }

    if (yshininess.IsScalar())
        Shininess = yshininess.as<float_type>();
}


ShaderProgramModel::ShaderProgramModel()
:  ShaderProgram(LOAD_RESOURCE(amethyst_gl_shaders_model_vert, Global.dir_shaders, "model.vert"),
                 LOAD_RESOURCE(amethyst_gl_shaders_model_frag, Global.dir_shaders, "model.frag")),
   vertexLoc_(GetAttribLocation("vertexPosition")),
   texcoordLoc_(GetAttribLocation("vertexTexCoord")),
   normalLoc_(GetAttribLocation("vertexNormal")),
   //projMatrixLoc_(GetUniformLocation("projMatrix")),
   modelViewMatrixLoc_(GetUniformLocation("modelViewMatrix")),
   normalMatrixLoc_(GetUniformLocation("normalMatrix")),
   mvpMatrixLoc_(GetUniformLocation("MVP")),
   texUnitLoc_(GetUniformLocation("baseTex")),
   lightPosLoc_(GetUniformLocation("Light.Position")),
   lightIntenLoc_(GetUniformLocation("Light.Intensity")),
   MaterialKaLoc_(GetUniformLocation("Material.Ka")),
   MaterialKdLoc_(GetUniformLocation("Material.Kd")),
   MaterialKsLoc_(GetUniformLocation("Material.Ks")),
   MaterialShineLoc_(GetUniformLocation("Material.Shininess"))
{
}

void ShaderProgramModel::setProjM(const TransMatrix& m_proj)
{
    UniformMatrix4f(projMatrixLoc_, m_proj);
}

void ShaderProgramModel::setModelViewM(const TransMatrix& m_modelview)
{
    UniformMatrix4f(modelViewMatrixLoc_, m_modelview);
}

void ShaderProgramModel::setNormalM(const NormalMatrix& m_normal)
{
    UniformMatrix3f(normalMatrixLoc_, m_normal);
}

void ShaderProgramModel::setMVP(const TransMatrix& m_mvp)
{
    UniformMatrix4f(mvpMatrixLoc_, m_mvp);
}

void ShaderProgramModel::setLight(const LightInfo& s_light)
{
    Uniform4f(lightPosLoc_, s_light.Position);
    Uniform3f(lightIntenLoc_, s_light.Intensity);
}

void ShaderProgramModel::setMaterial(const MaterialInfo& s_material)
{
    Uniform3f(MaterialKaLoc_, s_material.Ka);
    Uniform3f(MaterialKdLoc_, s_material.Kd);
    Uniform3f(MaterialKsLoc_, s_material.Ks);
    Uniform1f(MaterialShineLoc_, s_material.Shininess);
}


void ShaderProgramModel::setTexUnit(unsigned int  unit)
{
    Uniform1i(texUnitLoc_, unit);
}

unsigned int  ShaderProgramModel::getVertexLoc()
{
    return vertexLoc_.value;
}

unsigned int  ShaderProgramModel::getTexCoordLoc()
{
    return texcoordLoc_.value;
}

unsigned int  ShaderProgramModel::getNormalLoc()
{
    return normalLoc_.value;
}

} // namespace lib
} // namespace amethyst
