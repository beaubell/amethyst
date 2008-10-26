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

    virtual void render(unsigned int x, unsigned int y) = 0;

   protected:
    FTFont &font;

};


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


class UIW_Test : public UI_Window
{
   public:
    UIW_Test(UI &ui);
    void render(unsigned int x, unsigned int y);
};

} // namespace client
} // namespace amethyst


#endif // AMETHYST_CLIENT_UI_H
