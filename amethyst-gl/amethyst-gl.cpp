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
#include "scene_xml.h"
#include "timer.h"
#include "hud.h"
#include "physics.h"

#include "file.h"
#include "input.h"

#include <string>
#include <boost/lexical_cast.hpp>

#define TICK_INTERVAL    10


namespace amethyst
{
namespace client
{


Amethyst_GL::Amethyst_GL(const std::string &path_root)
  : ui("/spacefri.ttf"), //FIXME make not static
    input(new Input(*this)),
    time_scalar(100000.0f),
    manifest_(),
    connection(manifest_),
    paused(true),
    stride(60),
    simulation_interval(60),
    show_ui(true),
    show_hud(true)
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

  /// Setup Keyboard Shortcuts
  // Integration Control
  input->sig_kb[SDLK_SPACE].connect(bind(&Amethyst_GL::pause_toggle,this));
  input->sig_kb[SDLK_g].connect(bind(&Universe::toggle_gravity, &Global.universe));
  input->sig_kb[SDLK_4].connect(bind(&Universe::toggle_4thorder, &Global.universe));
  input->sig_kb[SDLK_EQUALS].connect(bind(&Amethyst_GL::simint_inc,this));
  input->sig_kb[SDLK_MINUS].connect(bind(&Amethyst_GL::simint_dec,this));
  input->sig_kb[SDLK_RIGHTBRACKET].connect(bind(&Amethyst_GL::stride_inc,this));
  input->sig_kb[SDLK_LEFTBRACKET].connect(bind(&Amethyst_GL::stride_dec,this));

  // Targeting Control
  input->sig_kb[SDLK_n].connect(scene_control_ship_next);
  input->sig_kb[SDLK_b].connect(scene_select_object_next);
  input->sig_kb[SDLK_t].connect(scene_target_object_next);
  input->sig_kb[SDLK_p].connect(bind(scene_xml_write,std::string("dump")));

  // GUI Control
  input->sig_kb[SDLK_F1].connect(bind(&Amethyst_GL::hud_toggle,this));
  input->sig_kb[SDLK_F2].connect(bind(&Amethyst_GL::ui_toggle,this));

  // History Buffer
  input->sig_kb[SDLK_c].connect(bind(&Amethyst_GL::state_save, this));
  input->sig_kb[SDLK_v].connect(bind(&Amethyst_GL::state_recall, this));
}


void Amethyst_GL::main_loop()
{
  Universe &universe = Global.universe;

  while(1)
  {
    Global.next_time = SDL_GetTicks() + TICK_INTERVAL;

    // Process Inputs
    int status = input->process_events();
    if (status) return;

    
    
    // Iterate Physics Engine
    Global.time_interval = SDL_GetTicks() - Global.time_ticks;
    Global.time_ticks += Global.time_interval;

    if (Global.time_interval > 0 && !paused)
    {
      //double simulation_interval = Global.time_interval / 1000.0 * time_scalar;
      universe.iterate(simulation_interval, stride);

      //universe.cl_integrate();
      //paused = true;
    }

    // Place L-points at their actual spots.
    Object::ptr sol = universe.object_find("Sol");
    Object::ptr earth = universe.object_find("Earth");
    Object::ptr solearthl1 = universe.object_find("S-E L1");
    Object::ptr solearthl2 = universe.object_find("S-E L2");
    if (sol != NULL && earth != NULL && solearthl1 != NULL && solearthl2 != NULL)
    {
      lib::placement_L1(*sol, *earth, *solearthl1);
      lib::placement_L2(*sol, *earth, *solearthl2);
    }
    
    /* update the screen */
    render();

    GLenum error = glGetError();
    if (error)
    {
      std::string log_msg =  "OpenGL Error Detected: " + boost::lexical_cast<std::string>(error);
      Global.log.add(log_msg);
    }
  }
}


void Amethyst_GL::render()
{
  // Clear the window with current clearing color
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  scene_render();

  sig_render_scene(Global.obj_view->location);
  // Display HUD XXX being replaced.

  if(show_hud)
    hud_render();

  if(show_ui)
    ui.render();

  // Do the buffer Swap
  SDL_GL_SwapBuffers();
}


void Amethyst_GL::start_net()
{
  Global.log.add("Net: Handing over control to Netwrok Thread...");
  Global.log.add("Thread: Starting Network Thread...");
  connection.run();
  Global.log.add("Thread: Network Thread Terminating, Nothing to do...");
}


void Amethyst_GL::pause_toggle()
{
  paused = !paused;
}


void Amethyst_GL::ui_toggle()
{
  show_ui = !show_ui;
}


void Amethyst_GL::hud_toggle()
{
  show_hud = !show_hud;
}


void Amethyst_GL::simint_inc()
{
  simulation_interval += 10.0;
  std::string log_msg = "Simulation Interval Increased to " + boost::lexical_cast<std::string>(simulation_interval) + " s";
  Global.log.add(log_msg);
}


void Amethyst_GL::simint_dec()
{
  simulation_interval -= 10.0;
  std::string log_msg = "Simulation Interval Decreased to " + boost::lexical_cast<std::string>(simulation_interval) + " s";
  Global.log.add(log_msg);
}


void Amethyst_GL::stride_inc()
{
  stride += 1;
  std::string log_msg = "Integration Stride Increased to " + boost::lexical_cast<std::string>(stride) + " iterations/run";
  Global.log.add(log_msg);
}


void Amethyst_GL::stride_dec()
{
  stride -= 1;
  std::string log_msg = "Integration Stride Decreased to " + boost::lexical_cast<std::string>(stride) + " iterations/run";
  Global.log.add(log_msg);
}


void Amethyst_GL::state_save()
{
  std::vector<cl::Event> wait_queue, new_events;
  size_t num_objects = Global.universe._object_list.size();
  Global.universe.iterate_gpu_tohistory(num_objects, Global.universe._current, 0, wait_queue, new_events);
  Global.universe.queue_rk4.finish();
  Global.universe.cl_copyfrgpu();
}


void Amethyst_GL::state_recall()
{
  std::vector<cl::Event> wait_queue, new_events;
  size_t num_objects = Global.universe._object_list.size();
  Global.universe.iterate_gpu_frhistory(num_objects, Global.universe._current, 0, wait_queue, new_events);
  Global.universe.queue_rk4.finish();
  Global.universe.cl_copyfrgpu();
}


} // namespace client
} // namespace amethyst






