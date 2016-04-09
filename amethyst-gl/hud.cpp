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
#include "hud_radial.h"
#include "hud_orbit.h"
#include "hud_objectnames.h"

#include "FTGL.h"
#include "FTGLTextureFont.h"

#include "shaderprog_hud.h"
#include "shaderprog_uifont.h"

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
static void hud_widget_memory(int x, int y);
static void hud_widget_location(int x, int y, const Cartesian_Vector &ref);
static void hud_widget_attitude(int x, int y, const Quaternion &reference);
static void hud_widget_camera(int x, int y);
static void hud_widget_fps(int x, int y);
static void hud_widget_select(const int x, const int y);
static void hud_widget_vectorbox(int x, int y, float xaxis, float yaxis, float zaxis);

static ftgl::FTFont* fonts[6];
static ShaderProgramHUD::sptr ui_shader;
static ShaderProgramFont::sptr uifont_shader;

static unsigned int frames = 0, benchmark = 0;
static float fps = 0.0f;

HUDRadial* hudradial;
HUDOrbit* hudorbit;
HUDObjectNames* hudobjlabels;

void hud_setup(void)
{
    std::string fontfile = Global.dir_fonts + "/spacefri.ttf";
    fonts[0] = new ftgl::FTGLTextureFont( fontfile.c_str());

    // Check to see if font loaded correctly
    if(fonts[0]->Error())
      printf("Font: %s TTF Font did not load!!\n", fontfile.c_str());
    else
      printf("Font: %s loaded\n", fontfile.c_str());

    fonts[0]->FaceSize( 13);

    ui_shader = std::make_shared<ShaderProgramHUD>();
    uifont_shader = std::make_shared<ShaderProgramFont>();

    hudradial = new HUDRadial(ui_shader);
    hudorbit = new HUDOrbit(ui_shader);
    hudobjlabels = new HUDObjectNames(*fonts[0], uifont_shader);
}

void hud_shutdown(void)
{
    delete fonts[0];
}

void hud_render(void)
{
    // unsigned int screen_y = Global.screen_y; /*unused*/

    glDisable( GL_DEPTH_TEST);

    // Radials from Sun Indicating Day/Month/Year
    if (hudradial)
      hudradial->render();

    // Orbit Indication for Earth around Sun and Moon around Earth
    if (hudorbit)
      hudorbit->render();

    if(hudobjlabels)
    {
        hudobjlabels->update();
        hudobjlabels->render();
    }
    

    //TEMP hud_widget_object_text();

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
*/
    //TEMP hud_widget_select(10,40);

    


    //TEMP hud_widget_vectorbox(0, 0, 0.5f, Global.throttle, -0.2f);

    glEnable( GL_DEPTH_TEST);
}


#ifdef HAVE_MALLOC_H
static void hud_widget_memory(int x, int y)
{
    struct mallinfo mstats = mallinfo();
    char status[100];
    snprintf(reinterpret_cast<char*>(&status), 50, "Memmory Blocks Allocated: %d",mstats.uordblks);
    //glRasterPos3f(-45.0f, 30.0f,-100.0f);
    //DEPRECATED glWindowPos2i(x, y);
    fonts[0]->Render(reinterpret_cast<char*>(&status));

    snprintf(reinterpret_cast<char*>(&status), 50, "Memmory Blocks Free: %d",mstats.fordblks);
    //glRasterPos3f(-45.0f, 29.0f,-100.0f);
    //DEPRECATED glWindowPos2i(x, y - 13);
    fonts[0]->Render(reinterpret_cast<char*>(&status));
}
#endif


static void hud_widget_location(int x, int y, const Cartesian_Vector &ref)
{
    char buffer[13];

    //DEPRECATED glWindowPos2i(x, y);
    fonts[0]->Render(" Location - ");

    //FIXME convert to use std::string
    snprintf(reinterpret_cast<char*>(&buffer), 12, "Z:%f", ref.x);
    //DEPRECATED glWindowPos2i(x += 110, y);
    fonts[0]->Render(reinterpret_cast<char*>(&buffer));

    snprintf(reinterpret_cast<char*>(&buffer), 12, "Y:%f", ref.y);
    //DEPRECATED glWindowPos2i(x += 100, y);
    fonts[0]->Render(reinterpret_cast<char*>(&buffer));

    snprintf(reinterpret_cast<char*>(&buffer), 12, "Z:%f", ref.z);
    //DEPRECATED glWindowPos2i(x += 100, y);
    fonts[0]->Render(reinterpret_cast<char*>(&buffer));
}


static void hud_widget_attitude(int x, int y, const Quaternion &ref)
{
    char buffer[20];

    //DEPRECATED glWindowPos2i(x, y);
    fonts[0]->Render(" Attitude - ");

    //FIXME convert to use std::string
    snprintf(reinterpret_cast<char*>(&buffer), 20, "W:%f", ref.w);
    //DEPRECATED glWindowPos2i(x += 110, y);
    fonts[0]->Render(reinterpret_cast<char*>(&buffer));

    snprintf(reinterpret_cast<char*>(&buffer), 20, "X:%f", ref.x);
    //DEPRECATED glWindowPos2i(x += 100, y);
    fonts[0]->Render(reinterpret_cast<char*>(&buffer));

    snprintf(reinterpret_cast<char*>(&buffer), 20, "Y:%f", ref.y);
    //DEPRECATED glWindowPos2i(x += 100, y);
    fonts[0]->Render(reinterpret_cast<char*>(&buffer));

    snprintf(reinterpret_cast<char*>(&buffer), 20, "Z:%f", ref.z);
    //DEPRECATED glWindowPos2i(x += 100, y);
    fonts[0]->Render(reinterpret_cast<char*>(&buffer));
}


static void hud_widget_camera(int x, int y) // FIXME
{
    char buffer[50];

    snprintf(reinterpret_cast<char*>(&buffer), 50, " Cam: X:%f deg, Y:%f deg, Z:%fx",Global.cam_yaw, Global.cam_pitch, Global.cam_zoom);
    //DEPRECATED glWindowPos2i(x, y);
    fonts[0]->Render(reinterpret_cast<char*>(&buffer));
}


static void hud_widget_select(const int x, const int y)
{
    const lib::Ship::sptr     ship = Global.ship;
    const lib::Object::sptr   view = Global.obj_view;
    const lib::Object::sptr   target = Global.obj_target;

    std::string text = "Piloting: " + ship->name;
    //DEPRECATED glWindowPos2i(x, y);
    fonts[0]->Render(text.c_str());

    text = "Viewing : " + view->name;
    //DEPRECATED glWindowPos2i(x, y - 13);
    fonts[0]->Render(text.c_str());

    if((view != target) && (target != Global.reference_object))
    {
        const double distance = lib::phys_distance(view->location, target->location);
        text = "Targeted: " + target->name;
        text += "  Distance: " + lexical_cast<std::string>(distance);

        const Cartesian_Vector speed = Cartesian_Vector(target->velocity - view->velocity);
        text += "  Speed: " + lexical_cast<std::string>(speed.magnitude());
        //DEPRECATED glWindowPos2i(x, y - 26);
        fonts[0]->Render(text.c_str());
    }
}



static void hud_widget_vectorbox(int /*x unused*/, int /*y unused*/, float xvector, float yvector, float zvector)
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


} // namespace client
} // namespace amethyst
