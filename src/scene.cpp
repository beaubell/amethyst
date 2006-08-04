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

#define TODEG(x)    x = x * 180 / M_PI
#define TORAD(x)    x = x / 180 * M_PI

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
  Cartesian_Vector &reference = Global.ship->location;
  Quaternion       &attitude  = Global.ship->attitude;

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

    // Clear the window with current clearing color
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glPushMatrix();

    //Camera
      //Camera initial values
  Cartesian_Vector raw_pos (0.0f, 0.0f, 0.0f);
  Cartesian_Vector raw_view(0.0f, 1e10, 0.0f);
  Cartesian_Vector raw_up  (0.0f, 0.0f, 1e10f);

      //Camera location in relation to ship
  Cartesian_Vector shipoffset(0.0f, -20.0f, 2.5f);

  {
        // Rotate Camera to ship's orientation
    Cartesian_Vector real_pos   = (QVRotate(attitude, (shipoffset + raw_pos )));
    Cartesian_Vector real_view  = (QVRotate(attitude, (shipoffset + raw_view)));
    Cartesian_Vector real_up    = (QVRotate(attitude, (shipoffset + raw_up  )));




      // Ring buffer (delays camera movement by a few frames)
    {
      Global.cam_num++;
      if(Global.cam_num > 8) Global.cam_num = 0;
      int cam_num = Global.cam_num;

      Global.cam_pos[cam_num] = real_pos;
      Global.cam_view[cam_num] = real_view;
      Global.cam_up[cam_num] = real_up;

      int view_num = cam_num - 7;
      if(view_num < 0) view_num = view_num + 9;
      real_pos  = Global.cam_pos[view_num];
      real_view = Global.cam_view[view_num];
      real_up   = Global.cam_up[view_num];
    }

      // Apply Camera
    gluLookAt(real_pos.x,  real_pos.y,  real_pos.z,
              real_view.x, real_view.y, real_view.z,
              real_up.x,   real_up.y,   real_up.z);
  }

    //Sky Box
  {
    glPushMatrix();
        //glDisable(GL_LIGHTING);
         //skybox();
    glCallList(Global.starfield_mdl);
    display_stars();
        //glEnable(GL_LIGHTING);
    glPopMatrix();
  }

    //Lights
  GLfloat lightPos[] = { 100.0f, 100.0f, 10000.0f, 1.0f };
  glLightfv(GL_LIGHT0, GL_POSITION, lightPos);



    //Draw Sun
  glPushMatrix();
  glDisable(GL_LIGHTING);
  glEnable(GL_TEXTURE_2D);

      //Move to ref position
  {
    Cartesian_Vector sun = Cartesian_Vector(100, 100, 10000);
    Cartesian_Vector temp = sun - reference;
    glTranslated(temp.x, temp.y, temp.z);
      //glTranslatef(100 , 100, 10000);
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

  display_hud();
    // Do the buffer Swap
  SDL_GL_SwapBuffers();
}


void scene_add_object(amethyst::Object *newobject)
{
    if (newobject)
      object_list.push_back(newobject);

}
