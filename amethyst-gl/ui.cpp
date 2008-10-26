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
#include <sstream>
#include <boost/bind.hpp>

namespace amethyst {
namespace client {


UI::UI(const std::string &fontfile)
{
    const std::string fontpath = Global.dir_fonts + fontfile;

    font_ = new FTGLTextureFont( fontpath.c_str());

    if(font_->Error())
      std::cout << "Font: %s TTF Font did not load!!" <<  fontpath << std::endl;
    else
      std::cout << "Font: %s loaded" << fontpath << std::endl;

    font_->FaceSize( 18);
    //font->Depth(20);
    font_->UseDisplayList(true);

}

UI::~UI()
{
    // Delete all wigets

    // Delete fonts
    delete font_;
}

void UI::render(void)
{
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glPushMatrix();

    int w_win = Global.screen_y; int h_win = Global.screen_x;

    // Set camera
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt( 0.0f, 0.0f, 1000.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

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

    /// Render each window
    std::for_each(windows_.begin(), windows_.end(),
       boost::bind(&UI_Window::render, _1, 200, 200));

    glPopMatrix();
    glPopAttrib();
}

FTFont& UI::get_font()
{
    return *font_;
}

void UI::add(UI_Window_ptr window)
{
    windows_.insert(window);
}

void UI::remove(UI_Window_ptr window)
{
    windows_.erase(window);
}

UI_Window::UI_Window(UI &ui)
    : font(ui.get_font())
{


}

UI_Window::~UI_Window()
{
}


UIW_FPS::UIW_FPS(UI &ui)
    : UI_Window(ui),
      frames(0),
      benchmark(0),
      fps(0.0f)
{
}

void UIW_FPS::render(unsigned int x, unsigned int y)
{
    std::string fpsstring;
    if(frames > 100)
    {
        unsigned int elapsed = SDL_GetTicks() - benchmark;
        fps = static_cast<float>(frames)/(static_cast<float>(elapsed)/1000.0f);
        benchmark += elapsed;
        frames = 0;
    }

    std::stringstream temp;
    std::string temp1;
    temp.precision(1);
    temp.setf(std::ios::fixed, std::ios::floatfield);
    temp << fps;

    temp >> temp1;
    fpsstring = "FPS: " + temp1;

    //glRasterPos2i(x, y);
    glTranslatef(x,y,0.0f);
    font.Render(fpsstring.c_str());

    frames++;
}


UIW_Test::UIW_Test(UI &ui)
    : UI_Window(ui)
{
}

void UIW_Test::render(unsigned int x, unsigned int y)
{

}


} // namespace client
} // namespace amethyst
