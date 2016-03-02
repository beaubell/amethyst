/***********************************************************************
 Amethyst-GL Module
  - FPS UI module declarations

 Authors (c):
 2008-2008 Beau V.C. Bellamy (beau@stellarnetservices.net)

 $Revision$
 $LastChangedDate$
 $LastChangedBy$
 ***********************************************************************/

#include "../ui.h"
#include "../ui_textbox.h"

namespace amethyst {
namespace client {
namespace module {

class UIW_Shipstat : public UI_Window
{
   public:
    UIW_Shipstat(UI &ui);
    void update();
    bool check_focus(unsigned short x, unsigned short y);

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

} // namespace module
} // namespace client
} // namespace amethyst