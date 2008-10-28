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
#include <set>

#include <boost/shared_ptr.hpp>

namespace amethyst {
namespace client {

class UI_Window;
typedef boost::shared_ptr<UI_Window> UI_Window_ptr;

class UI
{
   public:
    UI(const std::string &font);
    virtual ~UI();

    virtual void render(void);

    void add(UI_Window_ptr);
    void remove(UI_Window_ptr);

    FTFont &get_font();

   private:
    FTFont *font_;
    std::set<UI_Window_ptr> windows_;

};


class UI_Window
{
   public:
    UI_Window(UI &ui);
    virtual ~UI_Window();

    virtual void render(float min_x, float max_x, float min_y, float max_y) = 0;

   protected:
    FTFont &font;

};


} // namespace client
} // namespace amethyst


#endif // AMETHYST_CLIENT_UI_H
