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

    static GLuint textureID;

    font->FaceSize( 18);
    //font->Depth(20);
    font->UseDisplayList(true);

    //font->CharMap(ft_encoding_unicode);
/*
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 4, 4, 0, GL_RGB, GL_FLOAT, texture);
*/
}

UI::~UI()
{
    // Delete all wigets

    // Delete fonts
    delete font;
}

void UI::render(void)
{
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glPushMatrix();

    int w_win = Global.screen_y; int h_win = Global.screen_x;

    // Set camera
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt( 0.0, 0.0, (float)h_win/2.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);


    float light1_ambient[4]  = { 1.0, 1.0, 1.0, 1.0 };
    float light1_diffuse[4]  = { 1.0, 0.9, 0.9, 1.0 };
    float light1_specular[4] = { 1.0, 0.7, 0.7, 1.0 };
    float light1_position[4] = { -1.0, 1.0, 1.0, 0.0 };
    glLightfv(GL_LIGHT1, GL_AMBIENT,  light1_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE,  light1_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);
    glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
    glEnable(GL_LIGHT1);

    float front_emission[4] = { 0.3, 0.2, 0.1, 0.0 };
    float front_ambient[4]  = { 0.2, 0.2, 0.2, 0.0 };
    float front_diffuse[4]  = { 0.95, 0.95, 0.8, 0.0 };
    float front_specular[4] = { 0.6, 0.6, 0.6, 0.0 };
    glMaterialfv(GL_FRONT, GL_EMISSION, front_emission);
    glMaterialfv(GL_FRONT, GL_AMBIENT, front_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, front_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, front_specular);
    glMaterialf(GL_FRONT, GL_SHININESS, 16.0);
    glColor4fv(front_diffuse);

    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
    glColorMaterial(GL_FRONT, GL_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);

    glEnable(GL_LIGHTING);

    glEnable( GL_TEXTURE_2D);
    glDisable( GL_DEPTH_TEST);

    glNormal3f( 0.0, 0.0, 1.0);
    glColor3f( 1.0, 1.0, 1.0);

    glRasterPos2f(-0.40f, 0.35f);

    font->Render("ABCDEFGH");

    glPopMatrix();
    glPopAttrib();
}

//void UI::add();
//void UI::remove();



} // namespace client
} // namespace amethyst
