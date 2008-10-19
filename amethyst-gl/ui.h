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

#include "FTGL.h"
#include "FTGLTextureFont.h"

#include <string>

namespace amethyst {
namespace client {

class UI
{
   public:
    UI(const std::string &font);
    virtual ~UI();

    virtual void render(void);

    void add();
    void remove();

   private:
    FTFont* font;

};


} // namespace client
} // namespace amethyst

#endif // AMETHYST_CLIENT_UI_H
