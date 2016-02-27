#ifndef AMETHYST_CLIENT_UI_H
#define AMETHYST_CLIENT_UI_H

/***********************************************************************
 Amethyst-GL
  - OpenGL UI declarations

 Authors (c):
 2008-2008 Beau V.C. Bellamy (beau@stellarnetservices.net)

 $Revision$
 $LastChangedDate$
 $LastChangedBy$
 ***********************************************************************/

#include "opengl.h"
#include "ui_widget.h"
#include "FTGL.h"
#include "FTGLTextureFont.h"
#include "glm/glm.hpp"

#include <string>
#include <set>

#include <memory>

namespace amethyst {
namespace client {

class UI_Window;
typedef std::shared_ptr<UI_Window> UI_Window_ptr;

class UI
{
   public:
    UI(const std::string &font);
    virtual ~UI();
    
    void add(UI_Window_ptr);
    void remove(UI_Window_ptr);
    
    // Render UI
    virtual void render(void);

    // Focus functions
    bool check_focus(unsigned short x, unsigned short y, unsigned short but); // Uint16
    bool is_focused();
    
    ftgl::FTFont &get_font();

   private:
    ftgl::FTFont *font_;
    std::set<UI_Window_ptr> windows_;
    
};


class UI_Window
{
   public:
    typedef std::shared_ptr<UI_Window> sptr;
    UI_Window(UI &ui, const std::string &title);
    virtual ~UI_Window();

    virtual void resize(const glm::vec2 &newsize);
    virtual void setPosition(const glm::vec2 &newpos);
    virtual void render();
    virtual bool check_focus(unsigned short x, unsigned short y, unsigned short but);
    
    virtual void addWidget(UI_Widget::sptr newwidget);

    bool resizable;
    bool focused;
    std::string title;
   protected:
    ftgl::FTFont &font;
    glm::vec2 _position;
    glm::vec2 _size;

    uint _vao_frame[1]; // Vertex Array Objects Identifier
    uint _framebuffer[1];

    ftgl::TextHandle _titlehdl;
    
    std::set<UI_Widget::sptr> _widgets;
};

class UI_TextBox
{
   public:
    UI_TextBox(UI &ui, const std::string &title, const std::string &content, const glm::vec2 &position);
    virtual ~UI_TextBox();

    virtual void render();
    virtual bool check_focus(unsigned short x, unsigned short y, unsigned short but);
    

    bool resizable;
    bool focused;
    std::string title;

   protected:
    ftgl::FTFont &font;
    
    glm::vec2 _position;
    glm::vec2 _size;

    uint _text_[1]; // Vertex Array Objects Identifier
};

} // namespace client
} // namespace amethyst


#endif // AMETHYST_CLIENT_UI_H
