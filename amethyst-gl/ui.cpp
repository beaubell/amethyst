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
#include "opengl_shader.h"
#include "scene.h" // For set_camera

#include <iostream>
#include <sstream>
#include <boost/bind.hpp>

#include <cmath>

#include <boost/lexical_cast.hpp>


#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>

namespace amethyst {
namespace client {

using boost::lexical_cast;

// Shader Program Location
static ShaderProgram::ptr ui_shader;
static ShaderProgram::ptr uifont_shader;

// Vertex Attribute Locations
static ShaderProgram::AttribHDL uivertexLoc;
 
// Uniform variable Locations
static ShaderProgram::UniformHDL uicolorLoc;
static ShaderProgram::UniformHDL uiprojMatrixLoc, uiviewMatrixLoc;

// Vertex Attribute Locations
static ShaderProgram::AttribHDL fnvertexLoc;
static ShaderProgram::AttribHDL fntexcoordLoc;
 
// Uniform variable Locations
static ShaderProgram::UniformHDL fncolorLoc;
static ShaderProgram::UniformHDL fnprojMatrixLoc, fnviewMatrixLoc;
static ShaderProgram::UniformHDL fntexUnitLoc;

UI::UI(const std::string &fontfile)
{
    const std::string fontpath = Global.dir_fonts + fontfile;

    // Setup Shaders;
    ui_shader = ShaderProgram::ptr(new ShaderProgram("ui.vert", "ui.frag"));
    uifont_shader = ShaderProgram::ptr(new ShaderProgram("uifont.vert", "uifont.frag"));

    uivertexLoc     = ui_shader->GetAttribLocation("vertexData");

    uicolorLoc      = ui_shader->GetUniformLocation("color"); 
    uiprojMatrixLoc = ui_shader->GetUniformLocation("projMatrix");
    uiviewMatrixLoc = ui_shader->GetUniformLocation("viewMatrix");
    
    fnvertexLoc     = uifont_shader->GetAttribLocation("vertexData");
    fntexcoordLoc   = uifont_shader->GetAttribLocation("texcoordData");

    fncolorLoc      = uifont_shader->GetUniformLocation("color"); 
    fnprojMatrixLoc = uifont_shader->GetUniformLocation("projMatrix");
    fnviewMatrixLoc = uifont_shader->GetUniformLocation("viewMatrix");
    fntexUnitLoc    = uifont_shader->GetUniformLocation("baseTex");

    font_ = new FTGLTextureFont( fontpath.c_str());

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
    
    ui_shader = NULL;
}

void UI::render(void)
{

    float y = Global.screen_y; 
    float x = Global.screen_x;

    //glm::mat4 m_proj = glm::ortho(0.0f, static_cast<float>(w_win), 0.0f, static_cast<float>(h_win));
    //glm::mat4 m_proj = glm::ortho(-2e2f, 2e3f, -2e2f, 2e3f);
    glm::mat4 m_proj = glm::ortho(-1.0f, x, y, -1.0f);
    //glm::mat4 m_proj = glm::perspective((Global.fov), (fAspect), -1.0f, 5e9f);
    glm::mat4 m_identity(1.0);

    ui_shader->use();
    ui_shader->UniformMatrix4f(uiprojMatrixLoc, m_proj);
    ui_shader->UniformMatrix4f(uiviewMatrixLoc, m_identity);
    ui_shader->Uniform4f(uicolorLoc, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    
    uifont_shader->use();
    uifont_shader->UniformMatrix4f(fnprojMatrixLoc, m_proj);
    uifont_shader->UniformMatrix4f(fnviewMatrixLoc, m_identity);
    uifont_shader->Uniform4f(fncolorLoc, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    glUniform1i(fntexUnitLoc.value , 0);

    ui_shader->use();
    
    /// Render each window
    std::for_each(windows_.begin(), windows_.end(),
       boost::bind(&UI_Window::render, _1));
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
      std::cout  <<  "UI: GLError Reported: " << err << std::endl;
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
    : resizable(true),
      focused(false),
      title(newtitle),
      font(ui.get_font()),
      _titlehdl()
{
  
    _vao_frame[0] = 0;
    _framebuffer[0] = 0;

    resize(glm::vec2(100.0f, 100.0f));

    std::string entry = "Window created \"" + newtitle + "\" : size " + lexical_cast<std::string>(_size.x) + " x " + lexical_cast<std::string>(_size.y);
    Global.log.add(entry);

    // Prepare title text - FIXME
    //uifont_shader->use();
    font.Compose(title, _titlehdl);
    //_titlehdl.bind(uifont_shader->getHandle(), fnvertexLoc.value, fntexcoordLoc.value, fntexUnitLoc.value, 0);
    _titlehdl.bind(ui_shader->getHandle(), uivertexLoc.value, -1, -1, -1);
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
				0.0f    ,_size.y ,0.0f, 1.0f,
                                0.0f    ,0.0f    ,0.0f, 1.0f}; 
  
    if(_vao_frame[0] == 0 || _framebuffer[0] == 0)
    {
      glGenVertexArrays(1, _vao_frame);

      glBindVertexArray(_vao_frame[0]);

      // Generate buffer for frame;
      glGenBuffers(1, _framebuffer);
    }
    else {
      //std::cout << "check" << std::endl;
    }
    glBindVertexArray(_vao_frame[0]);
    glBindBuffer(GL_ARRAY_BUFFER, _framebuffer[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(window_frame), window_frame, GL_STATIC_DRAW);
    glEnableVertexAttribArray(uivertexLoc.value);
    glVertexAttribPointer(uivertexLoc.value, 4, GL_FLOAT, 0, 0, 0);
}

void UI_Window::setPosition(const glm::vec2 &newpos)
{
    _position = newpos;
}

void UI_Window::render()
{
    glm::mat4 m_identity;
    glm::vec3 v_window_pos(_position, 0.0f);
    glm::mat4 m_window = glm::translate(m_identity,v_window_pos);

    glLineWidth(1);
    GLushort background_idx[] = {0,3,1,2};
    GLushort frame_idx[] = {0,1,2,3,0};

    ui_shader->UniformMatrix4f(uiviewMatrixLoc, m_window);
    glBindVertexArray(_vao_frame[0]);
    
    //Draw Background
    ui_shader->Uniform4f(uicolorLoc, glm::vec4(0.0f, 0.1f, 0.0f, 0.5f));
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_SHORT, background_idx);

    //Draw Window Frame
    ui_shader->Uniform4f(uicolorLoc, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    glDrawElements(GL_LINE_STRIP, 5, GL_UNSIGNED_SHORT, frame_idx);

    // Place title font
    //uifont_shader->use();
    glm::mat4 m_textpos = glm::translate(m_window,glm::vec3(1.0f, 1.0f, 0.0f));
    //uifont_shader->UniformMatrix4f(fnviewMatrixLoc, m_textpos);
    ui_shader->UniformMatrix4f(uiviewMatrixLoc, m_textpos);
    _titlehdl.render();

    ui_shader->use();
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

} // namespace client
} // namespace amethyst
