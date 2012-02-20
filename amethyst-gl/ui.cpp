/***********************************************************************
 Amethyst-GL
  - OpenGL UI implementations

 Authors (c):
 2008-2008 Beau V.C. Bellamy (beau@stellarnetservices.net)

 $Revision$
 $LastChangedDate$
 $LastChangedBy$
 ***********************************************************************/

#include "opengl.h"
#include "ui.h"
#include "global.h"

#include <iostream>
#include <sstream>
#include <boost/bind.hpp>

#include <cmath>

#include <boost/lexical_cast.hpp>

namespace amethyst {
namespace client {

using boost::lexical_cast;

UI::UI(const std::string &fontfile)
{
    const std::string fontpath = Global.dir_fonts + fontfile;

    font_ = new FTGLTextureFont( fontpath.c_str());

    if(font_->Error())
      std::cout << "Font: %s TTF Font did not load!!" <<  fontpath << std::endl;
    else
      std::cout << "Font: %s loaded" << fontpath << std::endl;

    font_->FaceSize( 13);
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
//    glPushClientAttrib(GL_ALL_CLIENT_ATTRIB_BITS);
    glPushMatrix();

    int h_win = Global.screen_y; int w_win = Global.screen_x;

    // Set camera
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    //gluLookAt(0.0f, 0.0f, h_win*1.4, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

    // Start UI coords at bottom left
    gluLookAt(w_win/2,h_win/2,h_win*1.38, w_win/2, h_win/2, 0.0f, 0.0f, 1.0f, 0.0f);

    float light1_ambient[4]  = { 1.0f, 1.0f, 1.0f, 1.0f };
    float light1_diffuse[4]  = { 1.0f, 0.9f, 0.9f, 1.0f };
    float light1_specular[4] = { 1.0f, 0.7f, 0.7f, 1.0f };
    float light1_position[4] = { -1.0f, 1.0f, 1.0f, 0.0f };
    glLightfv(GL_LIGHT1, GL_AMBIENT,  light1_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE,  light1_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);
    glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
    glEnable(GL_LIGHT1);

    float front_emission[4] = { 0.3f, 0.2f, 0.1f, 0.0f };
    float front_ambient[4]  = { 0.2f, 0.2f, 0.2f, 0.0f };
    float front_diffuse[4]  = { 0.95f, 0.95f, 0.8f, 0.0f };
    float front_specular[4] = { 0.6f, 0.6f, 0.6f, 0.0f };
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

    glNormal3f( 0.0f, 0.0f, 1.0f);
    glColor4f( 1.0f, 1.0f, 1.0f, 1.0f);

    /// Render each window
    std::for_each(windows_.begin(), windows_.end(),
       boost::bind(&UI_Window::render, _1));

    glPopMatrix();
 //   glPopClientAttrib();
    glPopAttrib();
    
    // Debug stuff
    //int foo;
    //glGetIntegerv(GL_MODELVIEW_STACK_DEPTH, &foo);
    //std::cout << "Stack Depth" << foo;
}

bool UI::check_focus(unsigned short x, unsigned short y, unsigned short but)
{
    // Check each window to see if mouse is within it's bounds
    
    // Convert mouse coords to gl-coords
    y = Global.screen_y - y;
   
   
  
    // Else UI doesn't have focus.
    std::string log = "D: Mouse Click (" + lexical_cast<std::string>(but) + ") <" + lexical_cast<std::string>(x) + "," + lexical_cast<std::string>(y) + "";
    //log += " Converted   (" + lexical_cast<std::string>(posX) + "," + lexical_cast<std::string>(posY) + "," + lexical_cast<std::string>(posZ) +")";
    
    //Global.log.add(log);
    
    int err = glGetError();
    if(err)
      std::cout  <<  "GLError Reported: " << err << std::endl;
    return false;
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

UI_Window::UI_Window(UI &ui, const std::string &newtitle)
    : position_x(0),
      position_y(0),
      size_x(0),
      size_y(0),
      resizable(true),
      focused(false),
      title(newtitle),
      font(ui.get_font())
{


}

UI_Window::~UI_Window()
{
}

void UI_Window::render()
{
    //glPushMatrix();
    //glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);  // FIXME Seeing if this fixes the only on vertex pointer call problem
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    
    GLfloat window_frame[] =  { float(position_x+size_x),float(position_y-size_y),0.0f,
                                float(position_x+size_x),float(position_y)       ,0.0f,
                                float(position_x),       float(position_y)       ,0.0f,
                                float(position_x),       float(position_y-size_y),0.0f}; 

    GLushort frame_idx[] = {0,1,2,3,0};
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, window_frame);
    glDrawElements(GL_LINE_STRIP, 5, GL_UNSIGNED_SHORT, frame_idx);
    glDisableClientState(GL_VERTEX_ARRAY);
    
    glPushMatrix();
      
      glTranslatef(position_x + 2, position_y-12.0f, 0.0f);
      font.Render(title.c_str());
    glPopMatrix();
    
    glPopAttrib();
}

bool UI_Window::check_focus( unsigned short x, unsigned short y, unsigned short but)
{
   if ((x > position_x) && 
       (x < (position_x + size_x)) &&
       (y > position_y) &&
       (y < (position_y + size_y)) )
      return true;
   
   return false;
}

} // namespace client
} // namespace amethyst
