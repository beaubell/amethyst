/***********************************************************************
 Amethyst-GL
  - Scene rendering function implementations

 Authors (c):
 2006-2016 Beau V.C. Bellamy (bellamy.beau@gmail.com)
 ***********************************************************************/


#include <list>
#include <math.h>

#include "lib/vector.h"
#include "lib/orientation.h"
#include "lib/object.h"

#include "global.h"
#include "opengl.h"
#include "stars.h"
#include "hud.h"
#include "model.h"

#include "scene.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace amethyst {
namespace client {

//using lib::Object;
using lib::Cartesian_Vector;

std::list<lib::Object::sptr>  object_list;

double sun_rot = 0;


// Called to draw scene
void scene_render(const Eye eye)
{
  // Get Gobal State
  const Cartesian_Vector &reference = Global.obj_view->location;
  const Quaternion       &attitude  = Global.obj_view->attitude;

  Global.camera.setProjection(Screen(Global.screen_x, Global.screen_y));

  //Stars
  {
    // Set camera position without respect to camera zoom-out so that stars appear far away.
    auto tempdist = Global.camera.getDistance();
    Global.camera.setDistance(1.0);
    Global.camera.setAttitude(attitude);
    PVMatrix pvm = Global.camera.getMatrii(eye);

    // Render Stars
    stars_render(pvm.proj, pvm.view);

    // Resore camera distance
    Global.camera.setDistance(tempdist);
  }

  // Now consider camera zoom-out.
  Global.camera.setAttitude(attitude);
  PVMatrix pvm = Global.camera.getMatrii(eye);
  glm::mat4 m_model = glm::mat4(1);

#if 0
  //Draw Sun
  glPushMatrix();
    glDisable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    //Move to ref position
    {
      Cartesian_Vector sun = Cartesian_Vector(100.0, 100.0, -100000.0);
      Cartesian_Vector temp = sun - reference;
      glTranslated(temp.x, temp.y, temp.z);

     //Lights
      GLfloat lightPos[] = {(float)temp.x, (float)temp.y, (float)temp.z, 1.0f };
      glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    }

    //Rotate planet on axis
    //glRotated(sun_rot, 0.0, -1.0, 0.0);
    //sun_rot = sun_rot + 0.01;
    // glDisable(GL_COLOR_MATERIAL);
    //glCallList(Global.sun_mdl);
    //glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
  glPopMatrix();
#endif

  /// FIXME Special case for Sol 
  Scene_Object::sptr sol = std::dynamic_pointer_cast<Scene_Object>(Global.universe.object_find("Sol"));
  if (sol != NULL)
  {
    Cartesian_Vector temp = sol->location - reference;

    glm::mat4 m_sun = glm::translate(m_model, glm::vec3(temp.x, temp.y, temp.z));
    sol->render(pvm.proj, pvm.view, m_sun);
  }

  // Draw Objects in List.
  if(!object_list.empty())
  {
    auto obj1 = object_list.begin();
    do
    {
        if(sol != *obj1 && (*obj1)->model)
        {
            Cartesian_Vector temp = (*obj1)->location - reference;
            glm::mat4 m_mdlref = glm::translate(m_model, glm::vec3(temp.x, temp.y, temp.z));
            (*obj1)->model->render(pvm.proj, pvm.view, m_mdlref);
        }
        obj1++;
    }  while (obj1 != object_list.end());

  }

#if 0
  // Draw Network Objects
  for (int i = 0; i < Global.net_ships; i++)
  {
    glPushMatrix();
      Cartesian_Vector net_p = Global.net_ship[i].location - reference;
      //Move to ship position
      glTranslated(net_p.x, net_p.y, net_p.z);
      Quaternion       *net_q = &Global.net_ship[i].attitude;

      double theta = 2.0 * acos(net_q->w);
      TODEG(theta);

      //Rotate Ship
      glRotated(theta, net_q->x, net_q->y, net_q->z);
      {
        GLfloat fDiffLight[] =  { 1.0f, 1.0f, 0.0f };  // Green!!
        glLightfv(GL_LIGHT0, GL_DIFFUSE, fDiffLight);
      }
      //glCallList(Global.dlShip);  // FIXME XXX NETWORK OBJECTS NEED A MODEL

    glPopMatrix();
  }
#endif

}


void scene_add_object(lib::Object::sptr newobject)
{
    if (newobject)
      object_list.push_back(newobject);

}


void scene_select_object_next()
{
    lib::Object::sptr &selected = Global.obj_view;            // Reference to ship pointer
    Scene_Object::sptr &reference = Global.reference_object; // Pointer to reference_object

    if(!object_list.empty())
    {
        auto obj1 = Global.universe.list().begin();

        if(reference == selected)
        {
            selected = *obj1;
            return;
        }

        do
        {
            // Find Object
            if(selected == *obj1)
            {
                obj1++;
                if (obj1 == Global.universe.list().end())
                {
                    obj1 = Global.universe.list().begin();
                    selected = *obj1;
                }
                else
                    selected = *obj1;
                return;
            }

            obj1++;
        }  while (obj1 != Global.universe.list().end());

    } else
    {
        selected = reference;
    }

}


void scene_target_object_next()
{
    lib::Object::sptr& target   = Global.obj_target;            // Reference to target pointer
    Scene_Object::sptr& reference = Global.reference_object; // Pointer to reference_object

    if(!object_list.empty())
    {
        auto obj1 = Global.universe.list().begin();

        if(reference == target)
        {
            target = *obj1;
            return;
        }

        do
        {
            // Find Object
            if(target == *obj1)
            {
                obj1++;
                if (obj1 == Global.universe.list().end())
                {
                    obj1 = Global.universe.list().begin();
                    target = *obj1;
                }
                else
                    target = *obj1;
                return;
            }

            obj1++;
        }  while (obj1 != Global.universe.list().end());

    } else
    {
        target = reference;
    }

}

void scene_control_ship_next()
{
    lib::Ship::sptr& control  = Global.ship;            // Reference to control ship
    Scene_Ship::sptr& reference = Global.reference_ship; // Pointer to reference_ship

    if(!object_list.empty())
    {
        auto obj1 = Global.ships.begin();

        if(reference == control)
        {
            control = *obj1;
            return;
        }

        do
        {
            // Find Object
            if(control == *obj1)
            {
                obj1++;
                if (obj1 == Global.ships.end())
                {
                    //obj1 = Global.ships.begin();
                    //control = get_pointer(*obj1);
                    control = reference;
                }
                else
                    control = *obj1;
                return;
            }

            obj1++;
        }  while (obj1 != Global.ships.end());

    } else
    {
        control = reference;
    }

}

} // namespace amethyst
} // namespace client
