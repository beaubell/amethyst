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

#include <iostream>
#include <boost/lexical_cast.hpp>

#ifdef WIN32
#include "windows.h"
//#include "stdafx.h"
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
    void render();
    bool check_focus(unsigned short x, unsigned short y);

  private:
    unsigned int scroll_offset;
};

UIW_Log::UIW_Log(UI &ui)
    : UI_Window(ui, std::string("Message Log")),
      scroll_offset(0)
      
{
  int max_size = 10;
  
  position_x = 400;
  position_y = 400;
  size_x = 700;
  size_y = (static_cast<float>(max_size+1)*14.0f);
  
}

void UIW_Log::render()
{
    position_y = (static_cast<float>(11)*14.0f);
    position_x = Global.screen_x - size_x;
  
    UI_Window::render();
    glPushMatrix();
    glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);  // FIXME Seeing if this fixes the only on vertex pointer call problem
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    
    if(agl)
    {
        int max_size = 10;
        int start    = 0;
        int length   = Global.log.log().size();

        if (length > max_size)
            start = length - max_size - scroll_offset;

        for(int i = start; i < length; i++)
        {
            glPushMatrix();
            glTranslatef(position_x +20, position_y - (static_cast<float>(i-start+2)*14.0f), 0.0f);
            font.Render(Global.log.log()[i].c_str());
            glPopMatrix();
        }

    }
    glPopAttrib();
    glPopClientAttrib();
    glPopMatrix();
}

bool UIW_Log::check_focus(unsigned short /*x*/, unsigned short /*y*/)
{
  
  return false;
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
    //std::string log = "Module: " + module_name + " Initializing...";
    //Global.log.add(log);
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

