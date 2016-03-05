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
#include "../ui_textbox.h"
#include "../global.h"

#include <iostream>
#include <vector>

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
    void update();
    bool check_focus(unsigned short x, unsigned short y);

  private:
    unsigned int scroll_offset;
    std::vector<UI_TextBox::sptr> _logline;
};

UIW_Log::UIW_Log(UI &ui)
    : UI_Window(ui, std::string("Message Log")),
      scroll_offset(0)
      
{
  uint num_lines = 10;
  float line_height = 12.0f;
  
  for (uint i = 0; i < num_lines; i++)
  {
    UI_TextBox::sptr line(new UI_TextBox(ui.get_font(), ui.uifont_shader));
    _logline.push_back(line);
    _logline[i]->setPosition(glm::vec2(5.0f, static_cast<float>(i)*line_height));
    _logline[i]->setText(to_string(i));
    addWidget(_logline[i]);
  }
  
  float hsize = 700.0f;
  float vsize = static_cast<float>(num_lines+1)*line_height;
  
  setPosition(glm::vec2(-hsize-10.0f, -vsize-10.0f));
  resize(glm::vec2(hsize, vsize));
  
  update();
}

void UIW_Log::update()
{ 
    if(agl)
    {
        int max_size = 10;
        int start    = 0;
        int length   = Global.log.log().size();

        if (length > max_size)
            start = length - max_size - scroll_offset;

        for(int i = 0; i < length; i++)
        {
            _logline[i]->setText(Global.log.log()[i+start]);
        }

    }
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

