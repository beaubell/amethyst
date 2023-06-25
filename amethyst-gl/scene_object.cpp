/***********************************************************************
 Amethyst-GL
  - Scene Object implementations

 Authors (c):
 2008-2023 Beau V.C. Bellamy (bellamy.beau@gmail.com)
 ***********************************************************************/

#include "scene_object.h"

#include "model_manager.h"
#include "yaml-cpp/yaml.h"

#include <cmath>
#include <glm/gtc/matrix_transform.hpp>

namespace amethyst::client {


void Scene_Object_Base::render(const TransMatrix& /*m_proj*/, const TransMatrix& /*m_view*/, const TransMatrix& /*m_model*/) //, const lib::Cartesian_Vector& reference)
{

    // Move to object location
    //lib::Cartesian_Vector temp = location - reference;
    //glTranslated(temp.x, temp.y, temp.z);

   // Render Object
   //if(model)
   //  (model)->render(m_proj, m_view, m_model);

}


void Scene_Object::render(const TransMatrix& m_proj, const TransMatrix& m_view, const TransMatrix& m_model) //, const lib::Cartesian_Vector& reference)
{

    float theta = 2.0f * (float)acos(attitude.w);

    TransMatrix m_rotatemodel = glm::rotate(m_model, theta, glm::vec3(attitude.x, attitude.y, attitude.z));

    Scene_Object_Base::render(m_proj, m_view, m_rotatemodel);

}


void
Scene_Object::fromYAML(const YAML::Node& ynode) {

    using namespace YAML;
    Object::fromYAML(ynode);

    Node ymodel = ynode["model"];
    if (ymodel.IsScalar()) {
        auto modelstr = ymodel.as<std::string>();
        model = g_modelmanager.get(modelstr);
    }
}


void
Scene_Ship::fromYAML(const YAML::Node& ynode) {

    using namespace YAML;
    Object::fromYAML(ynode);

    Node ymodel = ynode["model"];
    if (ymodel && ymodel.IsScalar()) {
        auto modelstr = ymodel.as<std::string>();
        model = g_modelmanager.get(modelstr);
    }
}

} // namespace amethyst::client
