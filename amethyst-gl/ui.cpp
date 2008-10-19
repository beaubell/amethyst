/***********************************************************************
 Amethyst-GL
  - OpenGL UI implementations

 Authors (c):
 2008-2008 Beau V.C. Bellamy (beau@stellarnetservices.net)

 $Revision$
 $LastChangedDate$
 $LastChangedBy$
 ***********************************************************************/

#include "ui.h"
#include "global.h"

#include <iostream>

namespace amethyst {
namespace client {


UI::UI(const std::string &fontfile)
{
    const std::string fontpath = Global.dir_fonts + fontfile;

    font = new FTGLTextureFont( fontpath.c_str());

    if(font->Error())
      std::cout << "Font: %s TTF Font did not load!!" <<  fontpath << std::endl;
    else
      std::cout << "Font: %s loaded" << fontpath << std::endl;

    font->FaceSize( 13);

}

UI::~UI()
{
    // Delete all wigets

    // Delete fonts
    delete font;
}

void UI::render(void)
{
    gluOrtho2D(0, 600, 0, 800);
    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();

    glEnable( GL_TEXTURE_2D);
    //glDisable( GL_DEPTH_TEST);

    glColor3f( 1.0, 1.0, 1.0);
    glRasterPos2i( 20 , 20);
    font->Render("TEST TEST TEST");

}

//void UI::add();
//void UI::remove();



} // namespace client
} // namespace amethyst
