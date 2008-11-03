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
static const std::string module_name = "uiw_log";
static const std::string module_version = "0.0.1";

static UI_Window_ptr window;
static bool          module_active = false;

static Amethyst_GL*            agl = NULL;


/// UI_Window class derivative
class UIW_Log : public UI_Window
{
   public:
    UIW_Log(UI &ui);
    void render(float min_x, float max_x, float min_y, float max_y);

   private:
};

UIW_Log::UIW_Log(UI &ui)
    : UI_Window(ui)
{
}

void UIW_Log::render(const float min_x, const float max_x, const float min_y, const float max_y)
{
    glPushMatrix();
    if(agl)
    {
        int max_size = 10;
        int start    = 0;
        int length   = Global.log.log().size();

        if (length > max_size)
            start = length - max_size;

        glPushMatrix();
        glTranslatef(max_x - 500.0f, min_y + (static_cast<float>(max_size+1)*14.0f), 0.0f);
        font.Render("Message Log:");
        glPopMatrix();

        for(int i = start; i < length; i++)
        {
            glPushMatrix();
            glTranslatef(max_x - 490.0f, min_y + (static_cast<float>(length-i)*14.0f), 0.0f);
            font.Render(Global.log.log()[i].c_str());
            glPopMatrix();
        }

    }
    glPopMatrix();
}

} // namespace module
} // namespace client
} // namespace amethyst

using namespace amethyst;
using namespace amethyst::client;
using namespace amethyst::client::module;

void init(void);
void fini(void);

/// Windows Specific Module Initiators
#ifdef WIN32
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    if (fdwReason == DLL_PROCESS_ATTACH)
    {
        init();
        return true;
    }

    if (fdwReason == DLL_PROCESS_DETACH)
    {
        fini();
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
    std::string log = "Module: " + module_name + " Initializing...";
    Global.log.add(log);
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
    std::string log = "Module: " + module_name + " Activating...";
    Global.log.add(log);

    if (module_active == false)
    {
        agl = &agl_temp;

        window = UI_Window_ptr(new UIW_Log(agl->ui));
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

