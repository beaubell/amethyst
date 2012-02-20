/***********************************************************************
 Amethyst-GL Module
  - FPS UI module implementations

 Authors (c):
 2008-2008 Beau V.C. Bellamy (beau@stellarnetservices.net)

 $Revision$
 $LastChangedDate$
 $LastChangedBy$
 ***********************************************************************/

#include "mod_uiw_debug.h"

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
static const std::string module_name = "uiw_shipstat";
static const std::string module_version = "0.0.1";

static UI_Window_ptr window;
static bool          module_active = false;

static Amethyst_GL*            agl = NULL;



UIW_Shipstat::UIW_Shipstat(UI &ui)
    : UI_Window(ui, std::string("Ship Stats"))
      
{
  position_x = 0;
  position_y = 500;
  size_x = 520;
  size_y = 40;
}

void UIW_Shipstat::render()
{
    position_y = Global.screen_y - 20;
  
    UI_Window::render();
    glPushMatrix();
    glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);  // FIXME Seeing if this fixes the only on vertex pointer call problem
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    
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
        //temp.setf(std::ios::scientific, std::ios::floatfield);
        //temp.setf(std::ios::floatfield);
	
	float pos_y = position_y - 24.0f;
	
	temp << ref.x;
	temp >> temp2;
        buffer = "Location - X:" + temp2;
	glPushMatrix();
	glTranslatef(position_x+20, pos_y, 0.0f);
        font.Render(buffer.c_str());
	glPopMatrix();

	temp.clear();
	temp << ref.y;
	temp >> temp2;
	buffer = "Y:" + temp2;
	glPushMatrix();
	glTranslatef(position_x +210, pos_y, 0.0f);
        font.Render(buffer.c_str());
	glPopMatrix();

	glPushMatrix();
	temp.clear();
	temp << ref.z;
	temp >> temp2;
	buffer = "Z:" + temp2;
        glTranslatef(position_x +310, pos_y, 0.0f);
        font.Render(buffer.c_str());
        glPopMatrix();
	
	
	// Attitude 
	pos_y = pos_y - 10;
	temp.precision(6);
	
	glPushMatrix();
	temp.clear();
	temp << att.z;
	temp >> temp2;
	buffer = "Attitude - W:" + temp2;
        glTranslatef(position_x + 20, pos_y, 0.0f);
        font.Render(buffer.c_str());
        glPopMatrix();
	
	temp << att.x;
	temp >> temp2;
        buffer = "X:" + temp2;
	glPushMatrix();
	glTranslatef(position_x+ 210, pos_y, 0.0f);
        font.Render(buffer.c_str());
	glPopMatrix();

	temp.clear();
	temp << att.y;
	temp >> temp2;
	buffer = "Y:" + temp2;
	glPushMatrix();
	glTranslatef(position_x +310, pos_y, 0.0f);
        font.Render(buffer.c_str());
	glPopMatrix();

	glPushMatrix();
	temp.clear();
	temp << att.z;
	temp >> temp2;
	buffer = "Z:" + temp2;
        glTranslatef(position_x +410, pos_y, 0.0f);
        font.Render(buffer.c_str());
        glPopMatrix();
	
	
    }
    glPopAttrib();
    glPopClientAttrib();
    glPopMatrix();
}

bool UIW_Shipstat::check_focus(unsigned short x, unsigned short y)
{
  
  
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

