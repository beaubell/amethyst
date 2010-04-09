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

class UIW_FPS : public UI_Window
{
   public:
    UIW_FPS(UI &ui);
    void render(unsigned int x, unsigned int y);

   private:
    unsigned int frames;
    unsigned int benchmark;
    float fps;
};
