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

#include "opengl.h"
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
    
    void add(UI_Window_ptr);
    void remove(UI_Window_ptr);
    
    // Render UI
    virtual void render(void);

    // Focus functions
    bool check_focus(unsigned short x, unsigned short y, unsigned short but); // Uint16
    bool is_focused();
    
    FTFont &get_font();

   private:
    FTFont *font_;
    std::set<UI_Window_ptr> windows_;
    
};


class UI_Window
{
   public:
    UI_Window(UI &ui, const std::string &title);
    virtual ~UI_Window();

    virtual void render();
    virtual bool check_focus(unsigned short x, unsigned short y, unsigned short but);
    
    int position_x;
    int position_y;
    int size_x;
    int size_y;
    bool resizable;
    bool focused;
    std::string title;
   protected:
    FTFont &font;
    

};


} // namespace client
} // namespace amethyst


#endif // AMETHYST_CLIENT_UI_H
