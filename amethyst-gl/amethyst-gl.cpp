/***********************************************************************
 Amethyst-GL
  - Starup / Main Loop Functions

 Authors (c):
 2006-2008 Beau V.C. Bellamy (beau@stellarnetservices.net)

 $Revision$
 $LastChangedDate$
 $LastChangedBy$
 ***********************************************************************/


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>
//#include <cstdlib>
//#include <stdexcept>

#include "amethyst-gl.h"
#include "debug.h"
#include "global.h"
#include "opengl.h"
#include "joystick.h"
#include "net.h"
#include "texture.h"
#include "model.h"
#include "module.h"
#include "stars.h"
#include "thread.h"
#include "scene.h"
#include "timer.h"
#include "hud.h"

#include "file.h"
#include "input.h"

#define TICK_INTERVAL    10


namespace amethyst
{
namespace client
{


Amethyst_GL::Amethyst_GL(const std::string &path_root)
    : ui("/spacefri.ttf"), //FIXME make not static
      input(new Input(*this)),
      manifest_(),
      connection(manifest_),
      paused(true),
      time_scalar(100000.0f)
{

    /// DONT QUITE START THE NETWORK THREAD JUST YET
    //connection.start("127.0.0.1", "23232", "beau", "test");
    //net_thread = new boost::thread(boost::bind(&amethyst_gl::start_net, this));

    /// Create and add FPS widget to UI.
    //UI_Window_ptr win_fps(new UIW_FPS(ui));
    //ui.add(win_fps);

    /// Load Test UI modules and start it
    //if (module_manager.load("uiw_test"))
    //    module_manager.start("uiw_test", *this);

    if  (module_manager.load("uiw_fps"))
        module_manager.start("uiw_fps", *this);

    if  (module_manager.load("uiw_log"))
        module_manager.start("uiw_log", *this);

    if  (module_manager.load("uiw_debug"))
        module_manager.start("uiw_debug", *this);
    
    hud_setup();

    // Connect space bar to pause.
    input->sig_kb_space.connect(bind(&Amethyst_GL::pause_toggle,this));
}


void Amethyst_GL::main_loop()
{
    Universe &universe = Global.universe;

    while(1) {
    Global.next_time = SDL_GetTicks() + TICK_INTERVAL;

    // Process Inputs
    int status = input->process_events();
    if (status) return;

    
    
    // Iterate Physics Engine
    Global.time_interval = SDL_GetTicks() - Global.time_ticks;
    Global.time_ticks += Global.time_interval;

    if (Global.time_interval > 0 && !paused)
    {
      double simulation_interval = Global.time_interval / 1000.0 * time_scalar;
      universe.iterate(simulation_interval);

      Global.simulation_time += simulation_interval;
      //universe.cl_integrate();
      //paused = true;
    }

    /* update the screen */
    render();

    GLenum error = glGetError();
    if (error)
      std::cout << "glGetError: " << error << std::endl;
    }
}


void Amethyst_GL::render()
{
    // Clear the window with current clearing color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    scene_render();

    sig_render_scene(Global.obj_view->location);
    // Display HUD XXX being replaced.
    hud_render();

    ui.render();

    // Do the buffer Swap
    SDL_GL_SwapBuffers();
}


void Amethyst_GL::start_net()
{
	//namespace cout = std::cout;
	std::cout << "Net: Handing over control to Netwrok Thread...\n";
	std::cout << "Thread: Starting Network Thread..." << std::endl;
    connection.run();
	std::cout << "Thread: Network Thread Terminating, Nothing to do..." << std::endl;
}


void Amethyst_GL::pause_toggle()
{
    paused = !paused;
}

} // namespace client
} // namespace amethyst






