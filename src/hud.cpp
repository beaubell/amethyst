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

static FTFont* fonts[6];
//static FTGLPixmapFont* infoFont;

static unsigned int frames = 0, benchmark = 0;
static float fps = 0.0f;


void setup_hud(void)
{
    std::string fontfile = Global.dir_fonts + "/spacefri.ttf";
    fonts[0] = new FTGLPixmapFont( fontfile.c_str());

    printf("Font: %d\n", fonts[0]->Error());

    fonts[0]->FaceSize( 13);

}

void display_hud(void)
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
    char message[100];

    snprintf((char *)&message, 50, "Ship Stats");
    glWindowPos2i(10, screen_y - 46);
    fonts[0]->Render((char *)&message);

    snprintf((char *)&message, 50, " Location - X:%f, Y:%f, Z:%f",Global.ship->location.x, Global.ship->location.y,Global.ship->location.z);
    glWindowPos2i(10, screen_y - 56);
    fonts[0]->Render((char *)&message);

    Quaternion &quat = Global.ship->attitude;

    snprintf((char *)&message, 70, " Attitude - W:%f, X:%f, Y:%f, Z:%f",Global.ship->attitude.w, Global.ship->attitude.x, Global.ship->attitude.y, Global.ship->attitude.z);
    glWindowPos2i(10, screen_y - 66);
    fonts[0]->Render((char *)&message);

    snprintf((char *)&message, 50, " Cam: X:%f deg, Y:%f deg",Global.cam_yaw, Global.cam_pitch);
    glWindowPos2i(10, screen_y - 76);
    fonts[0]->Render((char *)&message);


    if(frames > 200)
    {
        unsigned int elapsed = SDL_GetTicks() - benchmark;
        fps = (float)frames/((float)elapsed/1000.0f);
        benchmark += elapsed;
        frames = 0;

    }

    char fpsstring[15];
    snprintf((char*)&fpsstring, 15, "FPS: %.1f",fps);
    //glRasterPos3f(-45.0f, 27.0f,-100.0f);
    glWindowPos2i(10, 4);
    fonts[0]->Render((char *)&fpsstring);

    frames++;

    glEnable( GL_LIGHTING);
    glEnable( GL_DEPTH_TEST);
}
/////
