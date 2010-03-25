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

namespace amethyst {
namespace client {
namespace module {

class UIW_Shipstat : public UI_Window
{
   public:
    UIW_Shipstat(UI &ui);
    void render();
    bool check_focus(unsigned short x, unsigned short y);

   private:

};

} // namespace module
} // namespace client
} // namespace amethyst