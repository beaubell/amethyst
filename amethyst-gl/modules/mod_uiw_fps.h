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

class UIW_FPS : public UI_Window
{
   public:
    UIW_FPS(UI &ui);
    virtual void update();

   private:
    unsigned int frames;
    unsigned int benchmark;
    float fps;
    UI_TextBox::sptr _tbfps;
};

} // namespace module
} // namespace client
} // namespace amethyst
