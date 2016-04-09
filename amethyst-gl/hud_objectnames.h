#pragma once

/***********************************************************************
 Amethyst-GL
  - HUD Object Labels Class declarations

 Authors (c):
 2016-2016 Beau V.C. Bellamy (beau@bellamy.beau@gmail.com)
 ***********************************************************************/

#include "hud_widget.h"
#include "shaderprog_uifont.h"

#include "lib/object.h"
#include "ui_textbox.h"

#include <tuple>
#include <forward_list>

namespace amethyst {
namespace client {


class HUDObjectNames : public HUDWidget
{
  public:
    typedef std::tuple<lib::Object::wptr, UI_TextBox::sptr> ObjectLabel;

    HUDObjectNames(ftgl::FTFont &font, ShaderProgramFont::sptr textshader);
    virtual ~HUDObjectNames(){};
    virtual void render();
    virtual void update();

  private:
    ftgl::FTFont &font_;
    ShaderProgramFont::sptr textshader_;
    std::forward_list<ObjectLabel> objectlabels_;
};


} // namespace lib
} // namespace amethyst
