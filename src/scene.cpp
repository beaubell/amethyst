/***************************************************************************
 *  Starfighter scene rendering functions                                  *
 *  (c) 2006 Beau V.C. Bellamy (beau@stellarnetservices.net)               *
 ***************************************************************************/

#include <list>
#include <math.h>

#include <amethyst/vector.h>
#include <amethyst/orientation.h>
#include <amethyst/object.h>

#include "global.h"
#include "opengl.h"
#include "stars.h"
#include "hud.h"

#include "SDL_opengl.h"

#include "scene.h"

#ifdef WIN32
#define M_PI 3.1415926535897932384626433832795f
#endif

#define TODEG(x)    x = x * 180.0f / M_PI
#define TORAD(x)    x = x / 180.0f * M_PI

using namespace amethyst;

std::list<amethyst::Object *>  object_list;

float sun_rot = 0;


static Cartesian_Vector QVRotate(Quaternion &q, const Cartesian_Vector &v)
{
  Quaternion t;

  t = (q*v)*q.Bar();
  return t.GetVector();
}


// Called to draw scene
// Fixme - Put Objects into some sort of linked list
void RenderScene(void)
{
  // Get Gobal State
  const Cartesian_Vector &reference = Global.ship->location;
  const Quaternion       &attitude  = Global.ship->attitude;

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // Clear the window with current clearing color
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  //Camera
  {
    // Get Camera Offsets
    float x = Global.cam_yaw;
    float y = Global.cam_pitch;

    // Convert to radians
    float x_rad = (x / 180.0f) * M_PI;
    float y_rad = (y / 180.0f) * M_PI;

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

    Quaternion Qz( cos(x_rad/2.0f), 0, 0, sin(x_rad/2.0f));
    Qz.normalize();
    Quaternion Qx( cos(y_rad/2.0f), sin(y_rad/2.0f), 0, 0 );
    Qx.normalize();

    Quaternion new_att = del_att * Qz * Qx;
    new_att.normalize();

    // Rotate Camera to ship's orientation
    {
        Cartesian_Vector raw_pos (0.0f, 0.0f, 0.0f);
        Cartesian_Vector raw_view(0.0f, 1.0f, 0.0f);
        Cartesian_Vector raw_up  (0.0f, 0.0f, 1.0f);

        //Camera location in relation to ship
        Cartesian_Vector shipoffset(0.0f, -20.0f, 2.5f);

        Cartesian_Vector real_pos   = (QVRotate(new_att, (shipoffset + raw_pos )));
        Cartesian_Vector real_view  = (QVRotate(new_att, (shipoffset + raw_view)));
        Cartesian_Vector real_up    = (QVRotate(new_att, (shipoffset + raw_up  )));

        // Apply Camera
        gluLookAt(real_pos.x,  real_pos.y,  real_pos.z,
                  real_view.x, real_view.y, real_view.z,
                  real_up.x,   real_up.y,   real_up.z);
    }
  } // Camera

  //Sky Box
  {
    glPushMatrix();
      //glCallList(Global.starfield_mdl);
      display_stars();
    glPopMatrix();
  }

  //Draw Sun
  glPushMatrix();
    glDisable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    //Move to ref position
    {
      Cartesian_Vector sun = Cartesian_Vector(100, 100, -100000);
      Cartesian_Vector temp = sun - reference;
      glTranslated(temp.x, temp.y, temp.z);

     //Lights
      GLfloat lightPos[] = { temp.x, temp.y, temp.z, 1.0f };
      glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    }

    //Rotate planet on axis
    glRotatef(sun_rot, 0, -1, 0);
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
    std::list<amethyst::Object *>::iterator obj1 = object_list.begin();
     do
    {
      glPushMatrix();
        // Move to object location
        Cartesian_Vector temp = (*obj1)->location - reference;
        glTranslated(temp.x, temp.y, temp.z);
        // Orient object
        Quaternion       *q = &(*obj1)->attitude;

        double theta = 2 * acos(q->w);
        TODEG(theta);

        glRotatef(theta, q->x, q->y, q->z);

        //glDisable(GL_COLOR_MATERIAL);

        // Render Object
        glCallList((GLint)(*obj1)->meta);

      glPopMatrix();
      obj1++;
    }  while (obj1 != object_list.end());

  }

  // Draw Network Objects
  for (int i = 0; i < Global.net_ships; i++)
  {
    glPushMatrix();
      Cartesian_Vector net_p = Global.net_ship[i].location - reference;
      //Move to ship position
      glTranslated(net_p.x, net_p.y, net_p.z);
      Quaternion       *net_q = &Global.net_ship[i].attitude;

      double theta = 2 * acos(net_q->w);
      TODEG(theta);

      //Rotate Ship
      glRotatef(theta, net_q->x, net_q->y, net_q->z);
      {
        GLfloat fDiffLight[] =  { 1.0f, 1.0f, 0.0f };  // Green!!
        glLightfv(GL_LIGHT0, GL_DIFFUSE, fDiffLight);
      }
      glCallList(Global.dlShip);

    glPopMatrix();
  }

  // Display HUD if windowpos gl extensions are suported
  if (glWindowPosSupported && glWindowPosEnabled)
    display_hud();

  // Do the buffer Swap
  SDL_GL_SwapBuffers();
}


void scene_add_object(amethyst::Object *newobject)
{
    if (newobject)
      object_list.push_back(newobject);

}
