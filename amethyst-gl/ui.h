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
#include "shaderprog_ui.h"
#include "shaderprog_uifont.h"
#include "ui_textbox.h"

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
    virtual void update();

    // Focus functions
    bool check_focus(unsigned short x, unsigned short y, unsigned short but); // Uint16
    bool is_focused();

    ftgl::FTFont &get_font();

    // Shader Program Location
    ShaderProgramUI::sptr   ui_shader;
    ShaderProgramFont::sptr uifont_shader;

   private:
    ftgl::FTFont *font_;
    std::set<UI_Window_ptr> windows_;

};


class UI_Window : public UI_Object
{
   public:
    typedef std::shared_ptr<UI_Window> sptr;
    typedef std::weak_ptr<UI_Window>   wptr;

    UI_Window(UI &ui, const std::string &title);
    virtual ~UI_Window();

    virtual void resize(const glm::vec2 &newsize);
    virtual void render(const TransMatrix& proj, const TransMatrix& window);
    virtual void update();
    virtual bool check_focus(unsigned short x, unsigned short y, unsigned short but);

    virtual void addWidget(UI_Widget::sptr newwidget);
    virtual void delWidget(UI_Widget::sptr newwidget);
    virtual void clearWidgets();

    bool resizable;
    bool focused;
    bool framed;
    std::string title;

    ShaderProgramUI::sptr ui_shader;

   protected:
    ftgl::FTFont &font;
    glm::vec2 _size;

    uint _vao_frame[1]; // Vertex Array Objects Identifier
    uint _vbo_frame[1];
    uint _ibo_frame[1];

    std::set<UI_Widget::sptr> _widgets;

    UI_TextBox _titlewidget;
};

} // namespace client
} // namespace amethyst


#endif // AMETHYST_CLIENT_UI_H
