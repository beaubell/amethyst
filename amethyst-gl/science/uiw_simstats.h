#ifndef AMETHYST_CLIENT_SIMSTATS_H
#define AMETHYST_CLIENT_SIMSTATS_H

/***********************************************************************
 Amethyst-GL
  - ??? FIXME ??? declarations

 Authors (c):
 2012-2012 Beau V.C. Bellamy (beau@stellarnetservices.net)

 $Revision$
 $LastChangedDate$
 $LastChangedBy$
 ***********************************************************************/

#include "../amethyst-gl.h"
#include "../ui_textbox.h"

namespace amethyst {
namespace client {

class UIW_SimStats : public UI_Window
{
  public:
    UIW_SimStats(Amethyst_GL &amgl, UI &ui);
    virtual void update();

  private:
    Amethyst_GL &_amgl;
    UI_TextBox::sptr _tbtime;
    UI_TextBox::sptr _tbCOMtitle;
    UI_TextBox::sptr _tbCOMx;
    UI_TextBox::sptr _tbCOMy;
    UI_TextBox::sptr _tbCOMz;
    UI_TextBox::sptr _tbVOMtitle;
    UI_TextBox::sptr _tbVOMx;
    UI_TextBox::sptr _tbVOMy;
    UI_TextBox::sptr _tbVOMz;

};


} // namespace client
} // namespace amethyst

#endif //AMETHYST_LIB_SIMSTATS_H
