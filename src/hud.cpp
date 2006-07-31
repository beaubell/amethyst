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

static FTFont* fonts[6];
//static FTGLPixmapFont* infoFont;


void setup_hud(void)
{
    std::string fontfile = Global.dir_fonts + "/spacefri.ttf";
    fonts[0] = new FTGLPixmapFont( fontfile.c_str());

    fonts[0]->FaceSize( 16);

}

void display_hud(void)
{
    Cartesian_Vector &reference = Global.ship->location;

    glColor3f(1.0f,1.0f,1.0f);
    glDisable( GL_DEPTH_TEST);
    glDisable( GL_LIGHTING);

    //glRasterPos2f( 5.0f , 5.0f);

    //fonts[0]->Render("Protocol 7: Engaged!");
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

    glEnable( GL_LIGHTING);
    glEnable( GL_DEPTH_TEST);
}
