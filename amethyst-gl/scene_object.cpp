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

namespace amethyst {
namespace client {

Scene_Object_Base::Scene_Object_Base(void)
{}

Scene_Object_Base::~Scene_Object_Base(void)
{}


void Scene_Object::render(const lib::Cartesian_Vector& reference)
{
    glPushMatrix();

    // Move to object location
    lib::Cartesian_Vector temp = location - reference;
    glTranslated(temp.x, temp.y, temp.z);


    double theta = 2.0 * acos(attitude.w);
    TODEG(theta);

    glRotated(theta, attitude.x, attitude.y, attitude.z);


   // Render Object
   if(meta)
           glCallList(reinterpret_cast<Model *>(meta)->dl);

    glPopMatrix();
}


void Scene_Ship::render(const lib::Cartesian_Vector& reference)
{
    glPushMatrix();

    // Move to object location
    lib::Cartesian_Vector temp = location - reference;
    glTranslated(temp.x, temp.y, temp.z);


    double theta = 2.0 * acos(attitude.w);
    TODEG(theta);

    glRotated(theta, attitude.x, attitude.y, attitude.z);


   // Render Object
   if(meta)
           glCallList(reinterpret_cast<Model *>(meta)->dl);

    glPopMatrix();
}


void Scene_Star::render(const lib::Cartesian_Vector& reference)
{

}


void Scene_Planet::render(const lib::Cartesian_Vector& reference)
{

}

} // namespace client
} // namespace amethyst
