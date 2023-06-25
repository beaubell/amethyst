/***********************************************************************
 Amethyst-GL
  - OpenGL UI implementations

 Authors (c):
 2008-2023 Beau V.C. Bellamy (bellamy.beau@gmail.com)
 ***********************************************************************/

#include "opengl.h"
#include "ui.h"
#include "global.h"
#include "shaderprog.h"
#include "shaderprog_ui.h"
#include "shaderprog_uifont.h"

#include <iostream>
#include <sstream>
#include <functional>
#include <cmath>

#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>

using namespace std::placeholders;

namespace amethyst::client {


UI::UI(const std::string &fontfile)
{
    const std::string fontpath = Global.dir_fonts + fontfile;

    // Setup Shaders;
    ui_shader = std::make_shared<ShaderProgramUI>();
    uifont_shader = std::make_shared<ShaderProgramFont>();

    font_ = new ftgl::FTGLTextureFont( fontpath.c_str());

    if(font_->Error())
      std::cout << "Font: %s TTF Font did not load!!" <<  fontpath << std::endl;
    else
      std::cout << "Font: %s loaded" << fontpath << std::endl;

    font_->FaceSize( 13);
    //font->Depth(20);
}

UI::~UI()
{
    // Delete all wigets

    // Delete fonts
    delete font_;
}

void UI::render()
{

    float y = Global.screen_y; 
    float x = Global.screen_x;

    glm::mat4 m_proj = glm::ortho(-1.0f, x, y, -1.0f);
    glm::mat4 m_identity(1.0);

    ui_shader->use();
    ui_shader->setProjM(m_proj);
    ui_shader->setViewM(m_identity);
    ui_shader->setColorV4(Color(1.0f, 1.0f, 1.0f, 1.0f));

    /// Render each window
    for(const auto& window: windows_)
    {
        if (window->isVisible())
	{
	    glm::vec2 window_position(window->getPosition());
	    
	    // Check for negative position.  Wrap if so.
	    window_position.x = (window_position.x < 0.0f)?(x+window_position.x):window_position.x;
	    window_position.y = (window_position.y < 0.0f)?(y+window_position.y):window_position.y;
	    
	    const glm::mat4 m_windowpos = glm::translate(m_identity, glm::vec3(window_position, 0.0f));
	    window->render(m_proj, m_windowpos);
	}
    }
}

void UI::update()
{
    /// Update each window
    std::for_each(windows_.begin(), windows_.end(),
       std::bind(&UI_Window::update, _1));
}

bool UI::check_focus(unsigned short x, unsigned short y, unsigned short but)
{
    // Check each window to see if mouse is within it's bounds
    
    // Convert mouse coords to gl-coords
    y = Global.screen_y - y;
   
    // Else UI doesn't have focus.
    std::string log = "D: Mouse Click (" + std::to_string(but) + ") <" + std::to_string(x) + "," + std::to_string(y) + "";
    //log += " Converted   (" + lexical_cast<std::string>(posX) + "," + lexical_cast<std::string>(posY) + "," + lexical_cast<std::string>(posZ) +")";

    //Global.log.add(log);

    int err = glGetError();
    if(err)
      std::cout  <<  "UI: GLError Reported: " << err << std::endl;
    return false;
}

ftgl::FTFont& UI::get_font()
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
    : resizable(true),
      focused(false),
      framed(true),
      title(newtitle),
      ui_shader(ui.ui_shader),
      font(ui.get_font()),
      _titlewidget(font, ui.uifont_shader)
{
    resize(glm::vec2(100.0f, 100.0f));

    std::string entry = "Window created \"" + newtitle + "\" : size " + std::to_string(_size.x) + " x " + std::to_string(_size.y);
    Global.log.add(entry);

    // Prepare title text
    _titlewidget.setText(newtitle);

}

UI_Window::~UI_Window()
{
}

void UI_Window::resize(const glm::vec2 &newsize)
{
     _size = newsize;
     GLfloat window_frame[] =  { 0.0f   ,0.0f    ,0.0f, 1.0f,
                                _size.x ,0.0f    ,0.0f, 1.0f,
                                _size.x ,_size.y ,0.0f, 1.0f,
                                 0.0f   ,_size.y ,0.0f, 1.0f,
                                0.0f    ,0.0f    ,0.0f, 1.0f}; 

    GLushort background_idx[] = {0,3,1,2};
    GLushort frame_idx[] = {0,1,2,3,0};

    vao_frame_.bind();
    
    vbo_frame_.bind();
    vbo_frame_.data(sizeof(window_frame), window_frame, GL_STATIC_DRAW);
    glEnableVertexAttribArray(ui_shader->getVertexLoc());
    glVertexAttribPointer(ui_shader->getVertexLoc(), 4, GL_FLOAT, 0, 0, 0);

    // IBO
    ibo_frame_.bind();
    ibo_frame_.data(sizeof(background_idx), background_idx, GL_STATIC_DRAW);
}

void UI_Window::render(const TransMatrix& m_proj, const TransMatrix& m_window)
{
    glLineWidth(1);

    glm::vec2 frameoffset(0.0f, 0.0f);

    if (framed)
    {
        ui_shader->use();
        ui_shader->setProjM(m_proj);
        ui_shader->setViewM(m_window);
        vao_frame_.bind();

        //Draw Background
        ui_shader->setColorV4(Color(0.0f, 0.1f, 0.0f, 0.5f));
        glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_SHORT, nullptr);

        //Draw Window Frame
        ui_shader->setColorV4(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
        glDrawArrays(GL_LINE_STRIP, 0, 5);

        //Draw Title
        glm::mat4 m_titlepos = glm::translate(m_window, glm::vec3(1.0f, 1.0f, 0.0f));
        _titlewidget.render(m_proj, m_titlepos);
	
	//Set Frame Offset due to title
	frameoffset = glm::vec2(1.0f, 15.0f);
    }
	
    //Widget Refence
    glm::mat4 m_widgetref = glm::translate(m_window, glm::vec3(frameoffset, 0.0f));

    //Render Widgets
    for(auto widit = _widgets.begin(); widit != _widgets.end(); widit++)
    {
        if ((*widit)->isVisible())
	{
	    glm::mat4 m_widgetpos = glm::translate(m_widgetref, glm::vec3((*widit)->getPosition(), 0.0f));
	    (*widit)->render(m_proj, m_widgetpos);  
	}
    }
}

void UI_Window::update()
{

}

bool UI_Window::check_focus( unsigned short x, unsigned short y, unsigned short but)
{
  /* FIXME
   if ((x > position_x) && 
       (x < (position_x + size_x)) &&
       (y > position_y) &&
       (y < (position_y + size_y)) )
      return true;
   */
   return false;
}

void UI_Window::addWidget(UI_Widget::sptr newwidget)
{ 
    _widgets.insert(newwidget);
}

void UI_Window::delWidget(UI_Widget::sptr byewidget)
{
    _widgets.erase(byewidget);
}

void UI_Window::clearWidgets()
{
    _widgets.clear();
}

} // namespace amethyst::client
