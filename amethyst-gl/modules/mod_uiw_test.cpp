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

static UI_Window_ptr window;
static bool          module_active = false;

static Amethyst_GL*            agl = NULL;


/// UI_Window class derivative
class UIW_Test : public UI_Window
{
   public:
    UIW_Test(UI &ui);
    ~UIW_Test();
    void render(float min_x, float max_x, float min_y, float max_y);

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


void UIW_Test::render(float min_x, float max_x, float min_y, float max_y)
{
    GLfloat frame[] =
        { 100.0f,100.0f,0.0f,  -100.0f,100.0f,0.0f,  -100.0f,-100.0f,0.0f,  100.0f,-100.0f,0.0f,
          200.0f,200.0f,0.0f,  -200.0f,200.0f,0.0f,  -200.0f,-200.0f,0.0f,  200.0f,-200.0f,0.0f,
          300.0f,300.0f,0.0f,  -300.0f,300.0f,0.0f,  -300.0f,-300.0f,0.0f,  300.0f,-300.0f,0.0f,
          400.0f,400.0f,0.0f,  -400.0f,400.0f,0.0f,  -400.0f,-400.0f,0.0f,  400.0f,-400.0f,0.0f,
          500.0f,500.0f,0.0f,  -500.0f,500.0f,0.0f,  -500.0f,-500.0f,0.0f,  500.0f,-500.0f,0.0f,
          750.0f,750.0f,0.0f,  -750.0f,750.0f,0.0f,  -750.0f,-750.0f,0.0f,  750.0f,-750.0f,0.0f,
         1000.0f,1000.0f,0.0f, -1000.0f,1000.0f,0.0f, -1000.0f,-1000.0f,0.0f, 1000.0f,-1000.0f,0.0f};

    GLushort frame_idx[] = {0,1,2,3,0,      4,5,6,7,4,  8,9,10,11,8,  12,13,14,15,12,
                            16,17,18,19,16,  20,21,22,23,20,  24,25,26,27,24};

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, frame);
    for (int i = 0; i < 7; i++)
        glDrawElements(GL_LINE_STRIP, 5, GL_UNSIGNED_SHORT, frame_idx + i*5);

    glDisableClientState(GL_VERTEX_ARRAY);

    for (int i = 1; i < 6; i++)
    {
        glPushMatrix();
        glTranslatef(static_cast<float>(i)*100.0f,0.0f,0.0f);

        std::string temp = "x:" + lexical_cast<std::string>(i*100);
        font.Render(temp.c_str());
        glPopMatrix();
    }

    for (int i = 1; i < 6; i++)
    {
        glPushMatrix();
        glTranslatef(0.0f,static_cast<float>(i)*100.0f,0.0f);

        std::string temp = "y:" + lexical_cast<std::string>(i*100);
        font.Render(temp.c_str());
        glPopMatrix();
    }

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

        window = UI_Window_ptr(new UIW_Test(agl->ui));
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
