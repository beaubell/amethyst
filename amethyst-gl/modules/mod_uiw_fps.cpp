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
#include "../global.h"
#include "SDL.h"

#include <iostream>

#ifdef WIN32
#include "windows.h"
#endif

namespace amethyst {
namespace client {
namespace module {

/// Module Static Objects
static const std::string module_name = "uiw_fps";
static const std::string module_version = "0.0.1";

static UI_Window_ptr window;
static bool          module_active = false;

static Amethyst_GL*            agl = NULL;


/// UI_Window class derivative
class UIW_FPS : public UI_Window
{
   public:
    UIW_FPS(UI &ui);
    void render();

   private:
    unsigned int frames;
    unsigned int benchmark;
    float fps;
};

UIW_FPS::UIW_FPS(UI &ui)
    : UI_Window(ui, std::string("FPS")),
      frames(0),
      benchmark(0),
      fps(0.0f)
{
  position_x = 0;
  position_y = 0;
}

void UIW_FPS::render()
{
    UI_Window::render();
    
    //glPushMatrix();
    //glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);  // FIXME Seeing if this fixes the only on vertex pointer call problem
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    
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

    glPushMatrix();
    font.Render(fpsstring.c_str());
    glPopMatrix();

    frames++;
    
    glPopAttrib();
    //glPopClientAttrib();
    //glPopMatrix();

}

} // namespace module
} // namespace client
} // namespace amethyst

using namespace amethyst;
using namespace amethyst::client;
using namespace amethyst::client::module;


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
void __attribute__ ((constructor)) init(void);
void __attribute__ ((destructor)) fini(void);
#endif

/// Main Initiators
void init(void)
{
    //std::string log = "Module: " + module_name + " Initializing...";
    //Global.log.add(log);

    //module = Module_ptr(new Module(module_name));

    //module->reg_start(&start);
    //module->reg_stop(&stop);

    //module_manager.reg(module);
}


void fini(void)
{
    //std::cout << "Module s: " << module_name << " Shutting Down..." << std::endl;
    // FIXME: module_name gets corrupted at this point...  Need to figure out why...

    std::cout << "Module Shutting Down..." << std::endl;
}


/// Module Entry Points
extern "C" bool mod_start(Amethyst_GL &agl_temp)
{
    std::cout << "Module: " << module_name << " Activating..." << std::endl;

    if (module_active == false)
    {
        agl = &agl_temp;

        window = UI_Window_ptr(new UIW_FPS(agl->ui));
        agl->ui.add(window);

        module_active = true;
        return true;
    }

    std::cout << "Module Error: " << module_name << " Already Activated!" << std::endl;
    return false;
}

extern "C" bool mod_stop()
{
    std::cout << "Module: " << module_name << " Deactivating..." << std::endl;

    if(module_active == true)
    {
        assert(agl != NULL);
        agl->ui.remove(window);
        window.reset();

        module_active = false;
        return true;
    }

    std::cout << "Module Error: " << module_name << " Already Deactivated!" << std::endl;
    return false;
}

extern "C" const std::string& mod_get_name()
{
    return module_name;
}

extern "C" const std::string& mod_get_version()
{
    return module_version;
}

extern "C" bool mod_is_active()
{
    return module_active;
}
