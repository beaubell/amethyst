#pragma once

/***********************************************************************
 Amethyst-GL
  - A window class to show stats on object being viewed.

 Authors (c):
 2016 Beau V.C. Bellamy (bellamy.beau@gmail.com)
 ***********************************************************************/

#include "../amethyst-gl.h"
#include "../ui_textbox.h"

namespace amethyst {
namespace client {

class UIW_View : public UI_Window
{
  public:
    UIW_View(Amethyst_GL &amgl, UI &ui);
    virtual void update();

  private:
    Amethyst_GL &_amgl;
    UI_TextBox::sptr tbViewing;
    UI_TextBox::sptr tbViewingMdl;
    UI_TextBox::sptr tbViewingTex;
    UI_TextBox::sptr tbTargeting;
    UI_TextBox::sptr tbPiloting;

};


} // namespace client
} // namespace amethyst
