/***********************************************************************
 Amethyst-GL Module
  - FPS UI module implementations

 Authors (c):
 2008-2008 Beau V.C. Bellamy (beau@stellarnetservices.net)

 $Revision$
 $LastChangedDate$
 $LastChangedBy$
 ***********************************************************************/

#include "mod_uiw_shipstats.h"

#include "../opengl.h"
#include "../module.h"
#include "../ui.h"
#include "../global.h"

#include <iostream>
#include <boost/lexical_cast.hpp>

#ifdef WIN32
#include "windows.h"
#endif

namespace amethyst {
namespace client {
namespace module {

/// Module Static Objects
static const std::string module_name = "uiw_shipstats";
static const std::string module_version = "0.0.1";

static UI_Window_ptr window;
static bool          module_active = false;

static Amethyst_GL*            agl = NULL;



UIW_Shipstat::UIW_Shipstat(UI &ui)
    : UI_Window(ui, std::string("Ship Stats")),
      _tbLocHead(new UI_TextBox(ui.get_font(), ui.uifont_shader)),
      _tbLocX(new UI_TextBox(ui.get_font(), ui.uifont_shader)),
      _tbLocY(new UI_TextBox(ui.get_font(), ui.uifont_shader)),
      _tbLocZ(new UI_TextBox(ui.get_font(), ui.uifont_shader)),
      _tbAttHead(new UI_TextBox(ui.get_font(), ui.uifont_shader)),
      _tbAttW(new UI_TextBox(ui.get_font(), ui.uifont_shader)),
      _tbAttX(new UI_TextBox(ui.get_font(), ui.uifont_shader)),
      _tbAttY(new UI_TextBox(ui.get_font(), ui.uifont_shader)),
      _tbAttZ(new UI_TextBox(ui.get_font(), ui.uifont_shader))
      
{
    setPosition(glm::vec2(10.0f, 10.0f));
    resize(glm::vec2(520.0f, 40.0f));

    glm::vec2 pos(10.0f, 2.0f);
    
    _tbLocHead->setText("Location - ");
    _tbLocHead->setPosition(pos);
    addWidget(_tbLocHead);
    
    pos.x += 100.0f;
    _tbLocX->setPosition(pos);
    addWidget(_tbLocX);
    
    pos.x += 100.0f;
    _tbLocY->setPosition(pos);
    addWidget(_tbLocY);
    
    pos.x += 100.0f;
    _tbLocZ->setPosition(pos);
    addWidget(_tbLocZ);
    
    pos = glm::vec2(10.0f, 12.0f);
	  
    _tbAttHead->setText("Attitude - ");
    _tbAttHead->setPosition(pos);
    addWidget(_tbAttHead);
	  
    pos.x += 100.0f;
    _tbAttW->setPosition(pos);
    addWidget(_tbAttW);
    
    pos.x += 100.0f;
    _tbAttX->setPosition(pos);
    addWidget(_tbAttX);
    
    pos.x += 100.0f;
    _tbAttY->setPosition(pos);
    addWidget(_tbAttY);
    
    pos.x += 100.0f;
    _tbAttZ->setPosition(pos);
    addWidget(_tbAttZ);
    
    update();
}

void UIW_Shipstat::update()
{
    if(agl)
    {
        using lib::Cartesian_Vector;
	using lib::Quaternion;
	using std::string;
	
	string buffer;
	const Cartesian_Vector &ref = Global.ship->location;
	const Quaternion &att = Global.ship->attitude;
	
	std::stringstream temp;
	std::string temp2;
        temp.precision(8);
	
	temp << ref.x;
	temp >> temp2;
        buffer = "X:" + temp2;
        _tbLocX->setText(buffer);

	temp.clear();
	temp << ref.y;
	temp >> temp2;
	buffer = "Y:" + temp2;
	_tbLocY->setText(buffer);

	temp.clear();
	temp << ref.z;
	temp >> temp2;
	buffer = "Z:" + temp2;
        _tbLocZ->setText(buffer);
	
	// Attitude 
	temp.precision(6);

	temp.clear();
	temp << att.z;
	temp >> temp2;
	buffer = "W:" + temp2;
        _tbAttW->setText(buffer);
	
	temp << att.x;
	temp >> temp2;
        buffer = "X:" + temp2;
        _tbAttX->setText(buffer);
	
	temp.clear();
	temp << att.y;
	temp >> temp2;
	buffer = "Y:" + temp2;
        _tbAttY->setText(buffer);

	temp.clear();
	temp << att.z;
	temp >> temp2;
	buffer = "Z:" + temp2;
        _tbAttZ->setText(buffer);
    }
}

bool UIW_Shipstat::check_focus(unsigned short /*x unused*/, unsigned short /*y unused*/)
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

        window = UI_Window_ptr(new UIW_Shipstat(agl->ui));
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

