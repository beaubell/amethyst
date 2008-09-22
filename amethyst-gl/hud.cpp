/***************************************************************************
 *  Heads up display function definitions                                  *
 *  (c) 2006 Beau V.C. Bellamy (beau@stellarnetservices.net)               *
 ***************************************************************************/

#include "hud.h"
#include "scene.h"
#include "global.h"

#include "FTGL.h"
#include "FTGLPixmapFont.h"
#include "SDL_opengl.h"
#include <string>

#ifdef HAVE_MALLOC_H
  #include <malloc.h>
#endif

#ifdef WIN32
#define snprintf _snprintf
#endif

#include <stdio.h>
#include <math.h> // for quaternion lenth calculation

// Forward Declarations
static void hud_widget_location(int x, int y, const Cartesian_Vector &ref);
static void hud_widget_attitude(int x, int y, const Quaternion &reference);
static void hud_widget_camera(int x, int y);
static void hud_widget_indicator_control();

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
    Cartesian_Vector &reference = Global.ship->location;

    glColor3f(1.0f,1.0f,1.0f);
    glDisable( GL_DEPTH_TEST);
    glDisable( GL_LIGHTING);

    //glRasterPos2f( 5.0f , 5.0f);

    if(!object_list.empty())
    {
        std::list<amethyst::Object *>::iterator obj1 = object_list.begin();

        do
        {
            glPushMatrix();

        // Move to object location
            Cartesian_Vector temp = (*obj1)->location - reference;
            glRasterPos3f(temp.x, temp.y, temp.z);

            fonts[0]->Render((*obj1)->name.c_str());

            glPopMatrix();
            obj1++;
        }  while (obj1 != object_list.end());

    }

        // Draw Network Objects
    for (int i = 0; i < Global.net_ships; i++)
    {
        glPushMatrix();

        Cartesian_Vector net_p = Global.net_ship[i].location - reference;
        glRasterPos3f(net_p.x, net_p.y, net_p.z);

        fonts[0]->Render(Global.net_ship[i].name.c_str());

        glPopMatrix();
    }
    //glMatrixMode(GL_MODELVIEW);
    //glLoadIdentity();


#ifdef HAVE_MALLOC_H
    struct mallinfo mstats = mallinfo();
    char status[100];
    snprintf((char*)&status, 50, "Memmory Blocks Allocated: %d",mstats.uordblks);
    //glRasterPos3f(-45.0f, 30.0f,-100.0f);
    glWindowPos2i(10, screen_y - 13);
    fonts[0]->Render((char *)&status);

    snprintf((char*)&status, 50, "Memmory Blocks Free: %d",mstats.fordblks);
    //glRasterPos3f(-45.0f, 29.0f,-100.0f);
    glWindowPos2i(10, screen_y - 26);
    fonts[0]->Render((char *)&status);
#endif

    glWindowPos2i(10, screen_y - 46);
    fonts[0]->Render((char *)"Ship Stats");

    // Display Location Information for Ship
    hud_widget_location(10, screen_y - 56, Global.ship->location);

    // Display Attitude Information for Ship
    hud_widget_attitude(10, screen_y - 66, Global.ship->attitude);

    // Display Camera Look Angles
    hud_widget_camera(10, screen_y - 76);

    hud_widget_indicator_control();

    if(frames > 100)
    {
        unsigned int elapsed = SDL_GetTicks() - benchmark;
        fps = (float)frames/((float)elapsed/1000.0f);
        benchmark += elapsed;
        frames = 0;

    }

    char fpsstring[30];
    snprintf((char*)&fpsstring, 30, "FPS: %.1f (Tick: %u )",fps, Global.time_ticks);
    //glRasterPos3f(-45.0f, 27.0f,-100.0f);
    glWindowPos2i(10, 4);
    fonts[0]->Render((char *)&fpsstring);

    frames++;

    glEnable( GL_LIGHTING);
    glEnable( GL_DEPTH_TEST);
}


static void hud_widget_location(int x, int y, const Cartesian_Vector &ref)
{
    char buffer[13];

    glWindowPos2i(x, y);
    fonts[0]->Render(" Location - ");

    snprintf((char *)&buffer, 12, "Z:%f", ref.x);
    glWindowPos2i(x += 110, y);
    fonts[0]->Render((char *)&buffer);

    snprintf((char *)&buffer, 12, "Y:%f", ref.y);
    glWindowPos2i(x += 100, y);
    fonts[0]->Render((char *)&buffer);

    snprintf((char *)&buffer, 12, "Z:%f", ref.z);
    glWindowPos2i(x += 100, y);
    fonts[0]->Render((char *)&buffer);
}


static void hud_widget_attitude(int x, int y, const Quaternion &ref)
{
    char buffer[20];

    glWindowPos2i(x, y);
    fonts[0]->Render((char *)" Attitude - ");

    snprintf((char *)&buffer, 20, "W:%f", ref.w);
    glWindowPos2i(x += 110, y);
    fonts[0]->Render((char *)&buffer);

    snprintf((char *)&buffer, 20, "X:%f", ref.x);
    glWindowPos2i(x += 100, y);
    fonts[0]->Render((char *)&buffer);

    snprintf((char *)&buffer, 20, "Y:%f", ref.y);
    glWindowPos2i(x += 100, y);
    fonts[0]->Render((char *)&buffer);

    snprintf((char *)&buffer, 20, "Z:%f", ref.z);
    glWindowPos2i(x += 100, y);
    fonts[0]->Render((char *)&buffer);
}


static void hud_widget_camera(int x, int y) // FIXME
{
    char buffer[50];

    snprintf((char *)&buffer, 50, " Cam: X:%f deg, Y:%f deg, Z:%fx",Global.cam_yaw, Global.cam_pitch, Global.cam_zoom);
    glWindowPos2i(x, y);
    fonts[0]->Render((char *)&buffer);
}


static void hud_widget_indicator_control()
{
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    GLfloat frame[] = { 1.0f,-0.1f,-0.1f,  1.0f,0.1f,-0.1f,  1.0f,0.1f,0.1f,  1.0f,-0.1f,0.1f,
                        -1.0f,-0.1f,-0.1f,  -1.0f,0.1f,-0.1f, -1.0f,0.1f,0.1f, -1.0f,-0.1f,0.1f,
                        -0.1f,1.0f,-0.1f,  0.1f,1.0f,-0.1f,  0.1f,1.0f,0.1f,  -0.1f,1.0f,0.1f,
                        -0.1f,-1.0f,-0.1f,  0.1f,-1.0f,-0.1f,  0.1f,-1.0f,0.1f,  -0.1f,-1.0f,0.1f,
                        -0.1f,-0.1f,1.0f, 0.1f,-0.1f,1.0f,  0.1f,0.1f,1.0f,  -0.1f,0.1f,1.0f,
                        -0.1f,-0.1f,-1.0f, 0.1f,-0.1f,-1.0f,  0.1f,0.1f,-1.0f,  -0.1f,0.1f,-1.0f};

    GLushort xbox[] = {0,1,2,3,0,4,5,6,7,4 ,5,1,2,6,7,3};
    GLushort ybox[] = {8,9,10,11,8,12,13,14,15,12,13,9,10,14,15,11};
    GLushort zbox[] = {16,17,18,19,16,20,21,22,23,20,21,17,18,22,23,19};

    GLfloat cframe[] = { 1.0f,-0.1f,-0.1f,  1.0f,0.1f,-0.1f,  1.0f,0.1f,0.1f,  1.0f,-0.1f,0.1f,
                        -1.0f,-0.1f,-0.1f,  -1.0f,0.1f,-0.1f, -1.0f,0.1f,0.1f, -1.0f,-0.1f,0.1f,
                        -0.1f,1.0f,-0.1f,  0.1f,1.0f,-0.1f,  0.1f,1.0f,0.1f,  -0.1f,1.0f,0.1f,
                        -0.1f,-1.0f,-0.1f,  0.1f,-1.0f,-0.1f,  0.1f,-1.0f,0.1f,  -0.1f,-1.0f,0.1f,
                        -0.1f,-0.1f,1.0f, 0.1f,-0.1f,1.0f,  0.1f,0.1f,1.0f,  -0.1f,0.1f,1.0f,
                        -0.1f,-0.1f,-1.0f, 0.1f,-0.1f,-1.0f,  0.1f,0.1f,-1.0f,  -0.1f,0.1f,-1.0f};

    float ythrust = Global.throttle;
    float ythrustp = 0.1f;
    float ythrustn = -0.1f;

    if (ythrust > 0) { ythrustp = ythrust*0.9f + 0.1f; ythrustn = 0.1f;}
    if (ythrust < 0) { ythrustn = ythrust*0.9f - 0.1f; ythrustp = -0.1f;}

    cframe[25] = cframe[28] = cframe [31] = cframe [34] = ythrustp;
    cframe[37] = cframe[40] = cframe [43] = cframe [46] = ythrustn;

    GLushort ythrustbox[] = {11,10,9,8, 9,13,12,8, 10,14,13,9, 11,15,14,10, 8,12,15,11, 12,13,14,15};

    glColor4f(0.0f, 0.0f, 0.0f, 1.0f);

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, frame);
    glDrawElements(GL_LINE_STRIP, 16, GL_UNSIGNED_SHORT, xbox);
    glDrawElements(GL_LINE_STRIP, 16, GL_UNSIGNED_SHORT, ybox);
    glDrawElements(GL_LINE_STRIP, 16, GL_UNSIGNED_SHORT, zbox);

    glVertexPointer(3, GL_FLOAT, 0, cframe);
    glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
    glDrawElements(GL_QUADS, 24, GL_UNSIGNED_SHORT, ythrustbox);

    glDisableClientState(GL_VERTEX_ARRAY);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);


    glPopAttrib();
}
