/***********************************************************************
 Amethyst-GL
  - Scene randering function implementations

 Authors (c):
 2006-2008 Beau V.C. Bellamy (beau@stellarnetservices.net)

 $Revision$
 $LastChangedDate$
 $LastChangedBy$
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


glm::dmat4 set_camera(const Quaternion &attitude, const double distance, const double eyeangle)
{

    // Get Camera Offsets
    double x = Global.cam_yaw;
    double y = Global.cam_pitch;

    // Convert to radians
    double x_rad = (double(x) / 180.0) * M_PI;
    double y_rad = (double(y) / 180.0) * M_PI;
    
    double eye_rad = (double(eyeangle) / 180.0) * M_PI;

    Quaternion del_att;

    // Ring buffer (delays camera movement by a few frames)
    {
        Global.cam_num++;
        if(Global.cam_num > 8) Global.cam_num = 0;
        int cam_num = Global.cam_num;

        Global.cam_att[cam_num] = attitude;

        int view_num = cam_num - 7;
        if(view_num < 0) view_num = view_num + 9;
        del_att  = Global.cam_att[view_num];

    }

    Quaternion Qz( cos(x_rad/2.0), 0.0, 0.0, sin(x_rad/2.0));
    Qz.normalize();
    Quaternion Qx( cos(y_rad/2.0), sin(y_rad/2.0), 0.0, 0.0 );
    Qx.normalize();

    Quaternion new_att = del_att * Qz * Qx;
    new_att.normalize();

    // Rotate Camera to ship's orientation
    {
        Cartesian_Vector raw_pos (0.0e11, 0.0, 0.0);
        Cartesian_Vector raw_view(0.0, 1.0e11, 0.0);
        Cartesian_Vector raw_up  (0.0, 0.0, 1.0e11);

        //Camera location in relation to ship
        Cartesian_Vector shipoffset(0.0, -distance, 0.0);
	Cartesian_Vector eyeoffset(-tan(eye_rad)*distance/2 ,0.0, 0.0);

        Cartesian_Vector real_pos   = (QVRotate(new_att, (shipoffset + raw_pos + eyeoffset)));
        Cartesian_Vector real_view  = (QVRotate(new_att, (shipoffset + raw_view + eyeoffset)));
        Cartesian_Vector real_up    = (QVRotate(new_att, (shipoffset + raw_up + eyeoffset)));

        // Apply Camera
        return glm::lookAt(glm::dvec3(real_pos.x, real_pos.y, real_pos.z),
                           glm::dvec3(real_view.x, real_view.y, real_view.z),
                           glm::dvec3(real_up.x,   real_up.y,   real_up.z));
    }


}


// Called to draw scene
// Fixme - Put Objects into some sort of linked list
void scene_render(const double eyeangle)
{
  // Get Gobal State
  const Cartesian_Vector &reference = Global.obj_view->location;
  const Quaternion       &attitude  = Global.obj_view->attitude;
  
  float x = Global.screen_x;
  float y = Global.screen_y;

  glm::mat4 m_proj = glm::perspective(glm::radians(30.0f), x/y, 1.0f, 10e15f);

  //Stars
  {
    // Set camera position without respect to camera zoom-out so that stars appear far away.
    glm::mat4 m_view = set_camera(attitude, 1.0, eyeangle);
    stars_render(m_proj, m_view);
  }

  // Now consider camera zoom-out.
  glm::mat4 m_view = set_camera(attitude, Global.cam_zoom, eyeangle);
  glm::mat4 m_model = glm::mat4(1);
  //glLoadMatrix(&m_view[0][0]); // FIXME - this is temporary. We'll eventually move this call higher in the chain as the gl fixed function calls are replaced.

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

  
  //DEPRECATED glDisable(GL_LIGHTING);
  { // Set light to white
    GLfloat fDiffLight[] =  { 1.0f, 0.9f, 0.9f, 1.0f };
    //DEPRECATEDglLightfv(GL_LIGHT0, GL_DIFFUSE, fDiffLight);
  }

  /// FIXME Special case for Sol 
  Scene_Object::sptr sol = std::dynamic_pointer_cast<Scene_Object>(Global.universe.object_find("Sol"));
  if (sol != NULL)
  {
    Cartesian_Vector temp = sol->location - reference;
    GLfloat lightPos[] = {(float)temp.x, (float)temp.y, (float)temp.z, 1.0f };
    //DEPRECATED glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    //sol->render(reference)
    glm::mat4 m_sun = glm::translate(m_model, glm::vec3(temp.x, temp.y, temp.z));
    sol->render(m_proj, m_view, m_sun);
  }

  //DEPRECATED glEnable(GL_LIGHTING);
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
            (*obj1)->model->render(m_proj, m_view, m_mdlref);
        }
        obj1++;
    }  while (obj1 != object_list.end());

  }

  //DEPRECATED glDisable(GL_LIGHTING);

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
