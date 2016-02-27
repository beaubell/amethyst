#ifndef AMETHYST_UI_TEXTBOX_H
#define AMETHYST_UI_TEXTBOX_H

#include "ui_widget.h"

namespace amethyst {
namespace client {

class UI_TextBox : public UI_Widget
{
  public:
    typedef std::shared_ptr<UI_TextBox> sptr;
    UI_TextBox();
   ~UI_TextBox();
   
    virtual void render();
    virtual void update();

    void setText(const std::string& text);

    std::string text;
};

} // namespace client
} // namespace amethyst

#endif // UI_WIDGET_H
