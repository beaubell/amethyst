/***********************************************************************
 Amethyst-GL
  - Heads up display function definitions

 Authors (c):
 2006-2008 Beau V.C. Bellamy (beau@stellarnetservices.net)

 $Revision$
 $LastChangedDate$
 $LastChangedBy$
 ***********************************************************************/

#include "hud.h"
#include "scene.h"
#include "global.h"
#include "opengl.h"
#include "lib/physics.h"
#include "lib/utility.h"

#include "FTGL.h"
#include "FTGLPixmapFont.h"
#include <string>
#include <boost/lexical_cast.hpp>

#ifdef HAVE_MALLOC_H
  #include <malloc.h>
#endif

#ifdef WIN32
#define snprintf _snprintf
#endif

#include <stdio.h>
#include <math.h> // for quaternion lenth calculation

namespace amethyst {
namespace client {

using lib::Cartesian_Vector;
using lib::Quaternion;
using boost::lexical_cast;


// Forward Declarations
static void hud_widget_object_text(void);
static void hud_widget_memory(int x, int y);
static void hud_widget_location(int x, int y, const Cartesian_Vector &ref);
static void hud_widget_attitude(int x, int y, const Quaternion &reference);
static void hud_widget_camera(int x, int y);
static void hud_widget_fps(int x, int y);
static void hud_widget_select(const int x, const int y);
static void hud_widget_vectorbox(int x, int y, float xaxis, float yaxis, float zaxis);
static void hud_radials();
static void hud_orbits();

static FTFont* fonts[6];
//static FTGLPixmapFont* infoFont;

static unsigned int frames = 0, benchmark = 0;
static float fps = 0.0f;

void hud_setup(void)
{
    std::string fontfile = Global.dir_fonts + "/spacefri.ttf";
    fonts[0] = new FTGLPixmapFont( fontfile.c_str());

    // Check to see if font loaded correctly
    if(fonts[0]->Error())
      printf("Font: %s TTF Font did not load!!\n", fontfile.c_str());
    else
      printf("Font: %s loaded\n", fontfile.c_str());

    fonts[0]->FaceSize( 13);

}

void hud_render(void)
{
    unsigned int screen_y = Global.screen_y;

    glColor3f(1.0f,1.0f,1.0f);
    glDisable( GL_DEPTH_TEST);
    glDisable( GL_LIGHTING);

    hud_radials();
    hud_orbits();
    
    hud_widget_object_text();

    #ifdef HAVE_MALLOC_H
//        hud_widget_memory(10, screen_y - 13);
    #endif

/*  Deprecated due to GUI code
    glWindowPos2i(10, screen_y - 46);
    fonts[0]->Render("Ship Stats");

    // Display Location Information for Ship
    hud_widget_location(10, screen_y - 56, Global.ship->location);
    // Display Attitude Information for Ship
    hud_widget_attitude(10, screen_y - 66, Global.ship->attitude);
    // Display Camera Look Angles
    hud_widget_camera(10, screen_y - 76);
    // FPS/Ticks indicator
    hud_widget_fps(10,4);
*/
    hud_widget_select(10,40);

    


    hud_widget_vectorbox(0, 0, 0.5f, Global.throttle, -0.2f);

    glEnable( GL_LIGHTING);
    glEnable( GL_DEPTH_TEST);
}


static void hud_widget_object_text(void)
{
    Cartesian_Vector &reference = Global.obj_view->location;

    // Print names on the objects
    if(!object_list.empty())
    {
        std::list<lib::Object::ptr>::iterator obj1 = Global.universe.list().begin();

        do
        {
            glPushMatrix();

        // Move to object location
            Cartesian_Vector temp = (*obj1)->location - reference;
            glRasterPos3d(temp.x, temp.y, temp.z);

            fonts[0]->Render((*obj1)->name.c_str());

            glPopMatrix();
            obj1++;
        }  while (obj1 != Global.universe.list().end());

    }

#if 0
    // Print names on network Objects
    for (int i = 0; i < Global.net_ships; i++)
    {
        glPushMatrix();

        Cartesian_Vector net_p = Global.net_ship[i].location - reference;
        glRasterPos3d(net_p.x, net_p.y, net_p.z);

        fonts[0]->Render(Global.net_ship[i].name.c_str());

        glPopMatrix();
    }
#endif

}


#ifdef HAVE_MALLOC_H
static void hud_widget_memory(int x, int y)
{
    struct mallinfo mstats = mallinfo();
    char status[100];
    snprintf(reinterpret_cast<char*>(&status), 50, "Memmory Blocks Allocated: %d",mstats.uordblks);
    //glRasterPos3f(-45.0f, 30.0f,-100.0f);
    glWindowPos2i(x, y);
    fonts[0]->Render(reinterpret_cast<char*>(&status));

    snprintf(reinterpret_cast<char*>(&status), 50, "Memmory Blocks Free: %d",mstats.fordblks);
    //glRasterPos3f(-45.0f, 29.0f,-100.0f);
    glWindowPos2i(x, y - 13);
    fonts[0]->Render(reinterpret_cast<char*>(&status));
}
#endif


static void hud_widget_location(int x, int y, const Cartesian_Vector &ref)
{
    char buffer[13];

    glWindowPos2i(x, y);
    fonts[0]->Render(" Location - ");

    //FIXME convert to use std::string
    snprintf(reinterpret_cast<char*>(&buffer), 12, "Z:%f", ref.x);
    glWindowPos2i(x += 110, y);
    fonts[0]->Render(reinterpret_cast<char*>(&buffer));

    snprintf(reinterpret_cast<char*>(&buffer), 12, "Y:%f", ref.y);
    glWindowPos2i(x += 100, y);
    fonts[0]->Render(reinterpret_cast<char*>(&buffer));

    snprintf(reinterpret_cast<char*>(&buffer), 12, "Z:%f", ref.z);
    glWindowPos2i(x += 100, y);
    fonts[0]->Render(reinterpret_cast<char*>(&buffer));
}


static void hud_widget_attitude(int x, int y, const Quaternion &ref)
{
    char buffer[20];

    glWindowPos2i(x, y);
    fonts[0]->Render(" Attitude - ");

    //FIXME convert to use std::string
    snprintf(reinterpret_cast<char*>(&buffer), 20, "W:%f", ref.w);
    glWindowPos2i(x += 110, y);
    fonts[0]->Render(reinterpret_cast<char*>(&buffer));

    snprintf(reinterpret_cast<char*>(&buffer), 20, "X:%f", ref.x);
    glWindowPos2i(x += 100, y);
    fonts[0]->Render(reinterpret_cast<char*>(&buffer));

    snprintf(reinterpret_cast<char*>(&buffer), 20, "Y:%f", ref.y);
    glWindowPos2i(x += 100, y);
    fonts[0]->Render(reinterpret_cast<char*>(&buffer));

    snprintf(reinterpret_cast<char*>(&buffer), 20, "Z:%f", ref.z);
    glWindowPos2i(x += 100, y);
    fonts[0]->Render(reinterpret_cast<char*>(&buffer));
}


static void hud_widget_camera(int x, int y) // FIXME
{
    char buffer[50];

    snprintf(reinterpret_cast<char*>(&buffer), 50, " Cam: X:%f deg, Y:%f deg, Z:%fx",Global.cam_yaw, Global.cam_pitch, Global.cam_zoom);
    glWindowPos2i(x, y);
    fonts[0]->Render(reinterpret_cast<char*>(&buffer));
}


static void hud_widget_fps(int x, int y)
{
    if(frames > 100)
    {
        unsigned int elapsed = SDL_GetTicks() - benchmark;
        fps = static_cast<float>(frames)/(static_cast<float>(elapsed)/1000.0f);
        benchmark += elapsed;
        frames = 0;
    }

    char fpsstring[30];
    snprintf(reinterpret_cast<char*>(&fpsstring), 30, "FPS: %.1f (Tick: %u )",fps, Global.time_ticks);

    glWindowPos2i(x, y);
    fonts[0]->Render(reinterpret_cast<char*>(&fpsstring));

    frames++;
}


static void hud_widget_select(const int x, const int y)
{
    const lib::Ship     &ship = *Global.ship;
    const lib::Object   &view = *Global.obj_view;
    const lib::Object &target = *Global.obj_target;

    std::string text = "Piloting: " + ship.name;
    glWindowPos2i(x, y);
    fonts[0]->Render(text.c_str());

    text = "Viewing : " + view.name;
    glWindowPos2i(x, y - 13);
    fonts[0]->Render(text.c_str());

    if((&view != &target) && (&target != &Global.reference_object))
    {
        const double distance = lib::phys_distance(view.location, target.location);
        text = "Targeted: " + target.name;
        text += "  Distance: " + lexical_cast<std::string>(distance);

        const Cartesian_Vector speed = Cartesian_Vector(target.velocity - view.velocity);
        text += "  Speed: " + lexical_cast<std::string>(speed.magnitude());
        glWindowPos2i(x, y - 26);
        fonts[0]->Render(text.c_str());
    }
}



static void hud_widget_vectorbox(int x, int y, float xvector, float yvector, float zvector)
{

    GLfloat frame[] = { 1.0f,-0.1f,-0.1f,  1.0f,0.1f,-0.1f,   1.0f,0.1f,0.1f,   1.0f,-0.1f,0.1f,
                       -1.0f,-0.1f,-0.1f, -1.0f,0.1f,-0.1f,  -1.0f,0.1f,0.1f,  -1.0f,-0.1f,0.1f,
                       -0.1f,1.0f,-0.1f,   0.1f,1.0f,-0.1f,   0.1f,1.0f,0.1f,  -0.1f,1.0f,0.1f,
                       -0.1f,-1.0f,-0.1f,  0.1f,-1.0f,-0.1f,  0.1f,-1.0f,0.1f, -0.1f,-1.0f,0.1f,
                       -0.1f,-0.1f,1.0f,   0.1f,-0.1f,1.0f,   0.1f,0.1f,1.0f,  -0.1f,0.1f,1.0f,
                       -0.1f,-0.1f,-1.0f,  0.1f,-0.1f,-1.0f,  0.1f,0.1f,-1.0f, -0.1f,0.1f,-1.0f};

    GLushort xframe[] = {0,1,2,3,0,4,5,6,7,4,5,1,2,6,7,3};
    GLushort yframe[] = {8,9,10,11,8,12,13,14,15,12,13,9,10,14,15,11};
    GLushort zframe[] = {16,17,18,19,16,20,21,22,23,20,21,17,18,22,23,19};

    GLfloat cframe[] = { 1.0f,-0.1f,-0.1f,  1.0f,0.1f,-0.1f,   1.0f,0.1f,0.1f,   1.0f,-0.1f,0.1f,
                        -1.0f,-0.1f,-0.1f, -1.0f,0.1f,-0.1f,  -1.0f,0.1f,0.1f,  -1.0f,-0.1f,0.1f,
                        -0.1f,1.0f,-0.1f,   0.1f,1.0f,-0.1f,   0.1f,1.0f,0.1f,  -0.1f,1.0f,0.1f,
                        -0.1f,-1.0f,-0.1f,  0.1f,-1.0f,-0.1f,  0.1f,-1.0f,0.1f, -0.1f,-1.0f,0.1f,
                        -0.1f,-0.1f,1.0f,   0.1f,-0.1f,1.0f,   0.1f,0.1f,1.0f,  -0.1f,0.1f,1.0f,
                        -0.1f,-0.1f,-1.0f,  0.1f,-0.1f,-1.0f,  0.1f,0.1f,-1.0f, -0.1f,0.1f,-1.0f};


    GLfloat xvectorp,yvectorp,zvectorp = 0.1f;
    GLfloat xvectorn,yvectorn,zvectorn = -0.1f;

    if (xvector > 0) { xvectorp = xvector*0.9f + 0.1f; xvectorn = 0.1f;}
    if (xvector < 0) { xvectorn = xvector*0.9f - 0.1f; xvectorp = -0.1f;}
    if (yvector > 0) { yvectorp = yvector*0.9f + 0.1f; yvectorn = 0.1f;}
    if (yvector < 0) { yvectorn = yvector*0.9f - 0.1f; yvectorp = -0.1f;}
    if (zvector > 0) { zvectorp = zvector*0.9f + 0.1f; zvectorn = 0.1f;}
    if (zvector < 0) { zvectorn = zvector*0.9f - 0.1f; zvectorp = -0.1f;}

    cframe[0]  = cframe[3]  = cframe [6]  = cframe [9]  = xvectorp;
    cframe[12] = cframe[15] = cframe [18] = cframe [21] = xvectorn;
    cframe[25] = cframe[28] = cframe [31] = cframe [34] = yvectorp;
    cframe[37] = cframe[40] = cframe [43] = cframe [46] = yvectorn;
    cframe[50] = cframe[53] = cframe [56] = cframe [59] = zvectorp;
    cframe[62] = cframe[65] = cframe [68] = cframe [71] = zvectorn;

    GLushort xvectorbox[] = {3,2,1,0,     1,5,4,0,     2,6,5,1,     3,7,6,2,     0,4,7,3,     4,5,6,7};
    GLushort yvectorbox[] = {11,10,9,8,   9,13,12,8,   10,14,13,9,  11,15,14,10, 8,12,15,11,  12,13,14,15};
    GLushort zvectorbox[] = {19,18,17,16, 17,21,20,16, 18,22,21,17, 19,23,22,18, 16,20,23,19, 20,21,22,23};

    //Render Widgit
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glPushMatrix();

    //glMatrixMode(GL_MODELVIEW);
    //glLoadIdentity();

    Quaternion       *q = &Global.ship->attitude;

    double theta = 2.0 * acos(q->w);
    TODEG(theta);

    glRotated(theta, q->x, q->y, q->z);

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, frame);
    glDrawElements(GL_LINE_STRIP, 16, GL_UNSIGNED_SHORT, xframe);
    glDrawElements(GL_LINE_STRIP, 16, GL_UNSIGNED_SHORT, yframe);
    glDrawElements(GL_LINE_STRIP, 16, GL_UNSIGNED_SHORT, zframe);

    glVertexPointer(3, GL_FLOAT, 0, cframe);
    glDrawElements(GL_QUADS, 24, GL_UNSIGNED_SHORT, xvectorbox);
    glDrawElements(GL_QUADS, 24, GL_UNSIGNED_SHORT, yvectorbox);
    glDrawElements(GL_QUADS, 24, GL_UNSIGNED_SHORT, zvectorbox);

    glDisableClientState(GL_VERTEX_ARRAY);

    glPopMatrix();
    glPopAttrib();
}

static void hud_radials()
{
  const unsigned int radials       = 365;
  const double       radial_length = 1e12;

  double radialvectors[radials + 3][2][3];
  float  radialcolors[radials + 3][2][3];

  double radial_angle = 2.0*M_PI/static_cast<double>(radials);

  for(unsigned int i = 0; i < radials; i++)
  {
    radialvectors[i][0][0] = 0;
    radialvectors[i][0][1] = 0;
    radialvectors[i][0][2] = 0;
    radialvectors[i][1][0] = cos(static_cast<double>(i)*radial_angle)*radial_length;
    radialvectors[i][1][1] = sin(static_cast<double>(i)*radial_angle)*radial_length;
    radialvectors[i][1][2] = 0;

    radialcolors[i][0][0] = radialcolors[i][1][0] = (i == 0)?1.0f:0.0f;
    radialcolors[i][0][1] = radialcolors[i][1][1] = (i == 32 || i == 60 || i == 91 || i == 121 || i == 152 || i == 182 || i == 213 || i == 244 || i == 274 || i == 305  || i == 335  )?1.0:0.0f;
    radialcolors[i][0][2] = radialcolors[i][1][2] = (i != 0)?1.0f:0.0f;
  }

  lib::Object::ptr earth = Global.universe.object_find("Earth");
  lib::Object::ptr probe1 = Global.universe.object_find("S-E L1 Probe");
  lib::Object::ptr probe2 = Global.universe.object_find("S-E L2 Probe");
  lib::Object::ptr sol = Global.universe.object_find("Sol");

  if (probe1 != NULL && probe2 != NULL && sol != NULL && earth != NULL)
  {
    lib::Object L1_exact;
    lib::placement_L1(*sol, *earth, L1_exact);

    lib::Object L2_exact;
    lib::placement_L2(*sol, *earth, L2_exact);

    lib::Cartesian_Vector diff = L2_exact.location - sol->location;
    unsigned int i = 365;
    radialvectors[i][0][0] = 0;
    radialvectors[i][0][1] = 0;
    radialvectors[i][0][2] = 0;
    radialvectors[i][1][0] = diff.x;
    radialvectors[i][1][1] = diff.y;
    radialvectors[i][1][2] = 0;

    radialcolors[i][0][0] = radialcolors[i][1][0] = 1.0f;
    radialcolors[i][0][1] = radialcolors[i][1][1] = 1.0f;
    radialcolors[i][0][2] = radialcolors[i][1][2] = 1.0f;

    lib::Cartesian_Vector L1_diff = L1_exact.location - sol->location;
    lib::Cartesian_Vector probe_diff = probe1->location - sol->location;
    i = 366;
    radialvectors[i][0][0] = L1_diff.x;
    radialvectors[i][0][1] = L1_diff.y;
    radialvectors[i][0][2] = L1_diff.z;
    radialvectors[i][1][0] = probe_diff.x;
    radialvectors[i][1][1] = probe_diff.y;
    radialvectors[i][1][2] = probe_diff.z;

    radialcolors[i][0][0] = radialcolors[i][1][0] = 1.0f;
    radialcolors[i][0][1] = radialcolors[i][1][1] = 1.0f;
    radialcolors[i][0][2] = radialcolors[i][1][2] = 1.0f;

    std::stringstream temp;
    std::string temp2;
    temp.precision(8);

    temp << (L1_exact.location - probe1->location).magnitude();
    temp >> temp2;
    std::string text = "L1 Probe Distance: " + temp2;
    glWindowPos2i(10, 80);
    fonts[0]->Render(text.c_str());

    temp.clear();
    temp2.clear();
    temp << (L2_exact.location - probe2->location).magnitude();
    temp >> temp2;
    text = "L2 Probe Distance: " + temp2;
    glWindowPos2i(10, 67);
    fonts[0]->Render(text.c_str());

  }
  glEnable(GL_FOG);

  float FogCol[4] = {0.0f, 0.0f, 0.0f, 0.0f};
  glFogfv(GL_FOG_COLOR, FogCol);
  glFogi(GL_FOG_MODE, GL_LINEAR); // Note the 'i' after glFog - the GL_LINEAR constant is an integer.
  glFogf(GL_FOG_START, 10.0);
  glFogf(GL_FOG_END, Global.cam_zoom*10.0);

  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_DOUBLE, 0, radialvectors);
  glColorPointer(3, GL_FLOAT, 0, radialcolors);

  // draw radials
  Cartesian_Vector &reference = Global.obj_view->location;

  // Place "Sol" at the origin of radials
  if (sol != NULL)
  {
    glPushMatrix();
    Cartesian_Vector temp = sol->location - reference;
    glTranslated(temp.x, temp.y, temp.z);
  
    glDrawArrays(GL_LINES, 0, radials*2 + 4); // Plus four for earth radial and L1/Probe line.
    glPopMatrix();
  }
  
  // deactivate vertex arrays after drawing
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisable(GL_FOG);
}

static void hud_orbits()
{
  const unsigned int orbitpoints = 1001;
  const double radial_angle = 2.0*M_PI/static_cast<double>(orbitpoints-1);

  double earthorbit[orbitpoints][3];
  double moonorbit[orbitpoints][3];

  for(unsigned int i = 0; i < orbitpoints; i++)
  {
    double massratio = 3.0035191795284299e-06;
    double semi = 149.598e9;
    double Re = semi * (1 - massratio);
    earthorbit[i][0] = cos(static_cast<double>(i)*radial_angle)*Re;
    earthorbit[i][1] = sin(static_cast<double>(i)*radial_angle)*Re;
    earthorbit[i][2] = 0;

    moonorbit[i][0] = cos(static_cast<double>(i)*radial_angle)*384.400e6;
    moonorbit[i][1] = sin(static_cast<double>(i)*radial_angle)*384.400e6;
    moonorbit[i][2] = 0;

  }

  glEnable(GL_FOG);

  float FogCol[4] = {0.0f, 0.0f, 0.0f, 0.0f};
  glFogfv(GL_FOG_COLOR, FogCol);
  glFogi(GL_FOG_MODE, GL_LINEAR); // Note the 'i' after glFog - the GL_LINEAR constant is an integer.
  glFogf(GL_FOG_START, 10.0);
  glFogf(GL_FOG_END, Global.cam_zoom*10.0);

  glEnableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);
  glColor3ub(255,255,255);

  // draw radials
  Cartesian_Vector &reference = Global.obj_view->location;

  // Place "Sol" at the origin of radials
  lib::Object::ptr sol = Global.universe.object_find("Sol");
  if (sol != NULL)
  {
    glPushMatrix();
    Cartesian_Vector temp = sol->location - reference;
    glTranslated(temp.x, temp.y, temp.z);
    glVertexPointer(3, GL_DOUBLE, 0, earthorbit);
    glDrawArrays(GL_LINE_STRIP, 0, orbitpoints);
    glPopMatrix();
  }

  lib::Object::ptr earth = Global.universe.object_find("Earth");
  if (earth != NULL)
  {
    glPushMatrix();
    Cartesian_Vector temp = earth->location - reference;
    glTranslated(temp.x, temp.y, temp.z);
    glVertexPointer(3, GL_DOUBLE, 0, moonorbit);
    glDrawArrays(GL_LINE_STRIP, 0, orbitpoints);
    glPopMatrix();
  }
  
  // deactivate vertex arrays after drawing
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisable(GL_FOG);
}


} // namespace client
} // namespace amethyst
