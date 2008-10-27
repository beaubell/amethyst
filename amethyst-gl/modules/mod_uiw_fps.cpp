/***********************************************************************
 Amethyst-GL Module
  - FPS UI module implementations

 Authors (c):
 2008-2008 Beau V.C. Bellamy (beau@stellarnetservices.net)

 $Revision$
 $LastChangedDate$
 $LastChangedBy$
 ***********************************************************************/

#include "../module.h"
#include "../ui.h"
#include "SDL.h"

#include <iostream>

#ifdef WIN32
#include "windows.h"
#endif

#if 0
namespace amethyst {
namespace client {
namespace module {

#ifndef WIN32
void __attribute__ ((constructor)) init(void);
void __attribute__ ((destructor)) fini(void);
#endif

/// UI_Window class derivative
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

/// Module Static Objects
static const std::string module_name = "uiwfps";
static Module_ptr module(new Module(module_name));

static UI_Window_ptr window;

/// Module function prototypes
static void start(Amethyst_GL &agl);
static void stop(Amethyst_GL &agl);

void init(void)
{
    std::cout << "Loading Module: " << module_name << std::endl;

    module->reg_start(&start);
    module->reg_stop(&stop);

    module_manager.reg(module);
}


void fini(void)
{
    module_manager.unreg(module);
}

static void start(Amethyst_GL &agl)
{
    window = UI_Window_ptr(new UIW_FPS(agl.ui));
    agl.ui.add(window);

}

static void stop(Amethyst_GL &agl)
{
    agl.ui.remove(window);
    window.reset();
}

UIW_FPS::UIW_FPS(UI &ui)
    : UI_Window(ui),
      frames(0),
      benchmark(0),
      fps(0.0f)
{
}

void UIW_FPS::render(unsigned int x, unsigned int y)
{
    std::string fpsstring;
    if(frames > 100)
    {
        unsigned int elapsed = SDL_GetTicks() - benchmark;
        fps = static_cast<float>(frames)/(static_cast<float>(elapsed)/1000.0f);
        benchmark += elapsed;
        frames = 0;
    }

    std::stringstream temp;
    std::string temp1;
    temp.precision(1);
    temp.setf(std::ios::fixed, std::ios::floatfield);
    temp << fps;

    temp >> temp1;
    fpsstring = "FPS: " + temp1;

    //glRasterPos2i(x, y);
    glTranslatef(x,y,0.0f);
    font.Render(fpsstring.c_str());

    frames++;
}

} // namespace module
} // namespace client
} // namespace amethyst


/// Windows Specific Module Initiators
#ifdef WIN32
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    if (fdwReason == DLL_PROCESS_ATTACH)
    {
        amethyst::client::module::init();
        return true;
    }

    if (fdwReason == DLL_PROCESS_DETACH)
    {
        amethyst::client::module::fini();
        return true;
    }

    return false;
}
#else
//void __attribute__ ((constructor)) amethyst::client::module::init(void);
//void __attribute__ ((destructor)) amethyst::client::module::fini(void);
#endif
#endif
