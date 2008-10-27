/***********************************************************************
 Amethyst-GL
  - UI Test Module implementations

 Authors (c):
 2008-2008 Beau V.C. Bellamy (beau@stellarnetservices.net)

 $Revision$
 $LastChangedDate$
 $LastChangedBy$
 ***********************************************************************/

#include "../amethyst-gl.h"
//#include "../ui.h"
//#include "SDL.h"

#include <iostream>

#ifdef WIN32
#include "windows.h"
#endif

namespace amethyst {
namespace client {
namespace module {

/// Module Static Objects
static const std::string module_name = "uiw_test";
static const std::string module_version = "0.0.1";

//static Module_ptr    module;
static UI_Window_ptr window;
static bool          module_active = false;

Amethyst_GL         *agl;



/// UI_Window class derivative
class UIW_Test : public UI_Window
{
   public:
    UIW_Test(UI &ui);
    ~UIW_Test();
    void render(unsigned int x, unsigned int y);

   private:
};

UIW_Test::UIW_Test(UI &ui)
    : UI_Window(ui)
{
    std::cout << "UI_Window_Test Object Created..." << std::endl;
}

UIW_Test::~UIW_Test()
{
    std::cout << "UI_Window_Test Object Destroyed..." << std::endl;
}


void UIW_Test::render(unsigned int x, unsigned int y)
{
    /// Render Test
}

} // namespace module
} // namespace client
} // namespace amethyst

using namespace amethyst;
using namespace amethyst::client;
using namespace amethyst::client::module;

/// Windows Specific Module Initiators
void init(void);
void fini(void);

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
    std::cout << "Module: " << module_name << " Initializing..." << std::endl;

    //module = Module_ptr(new Module(module_name));

    //module->reg_start(&start);
    //module->reg_stop(&stop);

    //module_manager.reg(module);
}


void fini(void)
{
    std::cout << "Module: " << module_name << " Shutting Down..." << std::endl;
    //module_manager.unreg(module);

    //module.reset();
}

/// Module Entry Points
extern "C" bool mod_start(Amethyst_GL &agl_temp)
{
    std::cout << "Module: " << module_name << " Activating..." << std::endl;

    agl = &agl_temp;

    window = UI_Window_ptr(new UIW_Test(agl->ui));
    agl->ui.add(window);

    return true; // FIXME - Return something meaningful
}

extern "C" bool mod_stop()
{
    std::cout << "Module: " << module_name << " Deactivating..." << std::endl;
    agl->ui.remove(window);
    window.reset();

    return true; // FIXME - Return something meaningful
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
