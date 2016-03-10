/***********************************************************************
 Amethyst-GL
  - ??? FIXME ??? implementations

 Authors (c):
 2008-2008 Beau V.C. Bellamy (beau@stellarnetservices.net)

 $Revision$
 $LastChangedDate$
 $LastChangedBy$
 ***********************************************************************/

#include "scene_object.h"
#include "global.h"
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>

namespace amethyst {
namespace client {

Scene_Object_Base::Scene_Object_Base(void)
{}

Scene_Object_Base::~Scene_Object_Base(void)
{}


void Scene_Object_Base::render(const TransMatrix& m_proj, const TransMatrix& m_view, const TransMatrix& m_model) //, const lib::Cartesian_Vector& reference)
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

    // Move to object location
    //lib::Cartesian_Vector temp = location - reference;
    //glTranslated(temp.x, temp.y, temp.z);


    float theta = 2.0 * acos(attitude.w);
    //TODEG(theta);

    //glRotated(theta, attitude.x, attitude.y, attitude.z);
   TransMatrix model = glm::rotate(m_model, theta, glm::vec3(attitude.x, attitude.y, attitude.z));

   Scene_Object_Base::render(m_proj, m_view, m_model);

}


} // namespace client
} // namespace amethyst
