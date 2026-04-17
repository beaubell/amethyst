#ifndef AMETHYST_CLIENT_MOD_UIW_SHIPSTATS_H
#define AMETHYST_CLIENT_MOD_UIW_SHIPSTATS_H

/***********************************************************************
 Amethyst-GL Module
  - FPS UI module declarations

 Authors (c):
 2008-2026 Beau V.C. Bellamy (beau@stellarnetservices.net)
 ***********************************************************************/

#include "../ui.h"
#include "../ui_textbox.h"

namespace amethyst::client::module {

class UIW_Shipstat : public UI_Window
{
   public:
    UIW_Shipstat(UI &ui);
    void update() override;

   private:
    UI_TextBox::sptr _tbLocHead;
    UI_TextBox::sptr _tbLocX;
    UI_TextBox::sptr _tbLocY;
    UI_TextBox::sptr _tbLocZ;
    UI_TextBox::sptr _tbAttHead;
    UI_TextBox::sptr _tbAttW;
    UI_TextBox::sptr _tbAttX;
    UI_TextBox::sptr _tbAttY;
    UI_TextBox::sptr _tbAttZ;

};

} // namespace amethyst::client::module

#endif // AMETHYST_CLIENT_MOD_UIW_SHIPSTATS_H
