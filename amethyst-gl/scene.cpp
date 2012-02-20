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

namespace amethyst {
namespace client {

//using lib::Object;
using lib::Cartesian_Vector;

std::list<Scene_Object_Base *>  object_list;

double sun_rot = 0;


void set_camera(const Quaternion &attitude, const double distance)
{

    // Get Camera Offsets
    double x = Global.cam_yaw;
    double y = Global.cam_pitch;

    // Convert to radians
    double x_rad = (double(x) / 180.0) * M_PI;
    double y_rad = (double(y) / 180.0) * M_PI;

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

        Cartesian_Vector real_pos   = (QVRotate(new_att, (shipoffset + raw_pos )));
        Cartesian_Vector real_view  = (QVRotate(new_att, (shipoffset + raw_view)));
        Cartesian_Vector real_up    = (QVRotate(new_att, (shipoffset + raw_up  )));

        // Apply Camera
        gluLookAt(real_pos.x,  real_pos.y,  real_pos.z,
                  real_view.x, real_view.y, real_view.z,
                  real_up.x,   real_up.y,   real_up.z);
    }


}


// Called to draw scene
// Fixme - Put Objects into some sort of linked list
void scene_render(void)
{
  // Get Gobal State
  const Cartesian_Vector &reference = Global.obj_view->location;
  const Quaternion       &attitude  = Global.obj_view->attitude;

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  set_camera(attitude, 1.0);

  //Stars
  {
    glPushMatrix();
      stars_render();
    glPopMatrix();
  }

  glLoadIdentity();

  set_camera(attitude, Global.cam_zoom);

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
    glRotated(sun_rot, 0.0, -1.0, 0.0);
    sun_rot = sun_rot + 0.01;
     glDisable(GL_COLOR_MATERIAL);
    glCallList(Global.sun_mdl);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
  glPopMatrix();


  { // Set light to white
    GLfloat fDiffLight[] =  { 1.0f, 0.9f, 0.9f, 1.0f };
    glLightfv(GL_LIGHT0, GL_DIFFUSE, fDiffLight);
  }


  // Draw Objects in List.
  if(!object_list.empty())
  {
    std::list<Scene_Object_Base *>::iterator obj1 = object_list.begin();
    do
    {
        (*obj1)->render(reference);
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


void scene_add_object(Scene_Object_Base *newobject)
{
    if (newobject)
      object_list.push_back(newobject);

}


void scene_select_object_next()
{
    lib::Object* &selected = Global.obj_view  ;            // Reference to ship pointer
    lib::Object* reference = &Global.reference_object; // Pointer to reference_object

    if(!object_list.empty())
    {
        std::list<lib::Object *>::iterator obj1 = Global.universe.list().begin();

        if(reference == selected)
        {
            selected = *obj1;
            return;
        }

        do
        {
            // Find Object
            if(selected == (*obj1))
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
    lib::Object* &target   = Global.obj_target;            // Reference to target pointer
    lib::Object* reference = &Global.reference_object; // Pointer to reference_object

    if(!object_list.empty())
    {
        std::list<lib::Object *>::iterator obj1 = Global.universe.list().begin();

        if(reference == target)
        {
            target = *obj1;
            return;
        }

        do
        {
            // Find Object
            if(target == (*obj1))
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
    lib::Ship* &control  = Global.ship;            // Reference to control ship
    lib::Ship* reference = &Global.reference_ship; // Pointer to reference_ship

    if(!object_list.empty())
    {
        std::set<Scene_Ship_ptr>::iterator obj1 = Global.ships.begin();

        if(reference == control)
        {
            control = get_pointer(*obj1);
            return;
        }

        do
        {
            // Find Object
            if(control == get_pointer(*obj1))
            {
                obj1++;
                if (obj1 == Global.ships.end())
                {
                    //obj1 = Global.ships.begin();
                    //control = get_pointer(*obj1);
                    control = reference;
                }
                else
                    control = get_pointer(*obj1);
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
