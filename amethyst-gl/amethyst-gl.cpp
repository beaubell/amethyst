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
    stride(100),
    simulation_interval(200),
    show_ui(true),
    show_hud(true),
    fullscreen(false),
    stereo(false),
    fbleft_("left"),
    fbright_("right"),
    texleft_(),
    texright_(),
    rbleft_(),
    rbright_(),
    fbshader_("fb.vert","fb.frag"),
    shadereye_(fbshader_.GetUniformLocation("eye"))
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

  if  (module_manager.load("uiw_shipstats"))
      module_manager.start("uiw_shipstats", *this);

  hud_setup();

  /// Setup Keyboard Shortcuts
  // Integration Control
  input->sig_kb[SDL_SCANCODE_SPACE].connect(bind(&Amethyst_GL::pause_toggle,this));
  input->sig_kb[SDL_SCANCODE_G].connect(bind(&Universe::toggle_gravity, &Global.universe));
  input->sig_kb[SDL_SCANCODE_4].connect(bind(&Universe::toggle_4thorder, &Global.universe));
  input->sig_kb[SDL_SCANCODE_EQUALS].connect(bind(&Amethyst_GL::simint_inc,this));
  input->sig_kb[SDL_SCANCODE_MINUS].connect(bind(&Amethyst_GL::simint_dec,this));
  input->sig_kb[SDL_SCANCODE_RIGHTBRACKET].connect(bind(&Amethyst_GL::stride_inc,this));
  input->sig_kb[SDL_SCANCODE_LEFTBRACKET].connect(bind(&Amethyst_GL::stride_dec,this));

  // Targeting Control
  input->sig_kb[SDL_SCANCODE_N].connect(scene_control_ship_next);
  input->sig_kb[SDL_SCANCODE_B].connect(scene_select_object_next);
  input->sig_kb[SDL_SCANCODE_T].connect(scene_target_object_next);
  input->sig_kb[SDL_SCANCODE_P].connect(bind(scene_xml_write,std::string("dump")));

  // GUI Control
  input->sig_kb[SDL_SCANCODE_F1].connect(bind(&Amethyst_GL::hud_toggle,this));
  input->sig_kb[SDL_SCANCODE_F2].connect(bind(&Amethyst_GL::ui_toggle,this));
  input->sig_kb_ctl[SDL_SCANCODE_F].connect(bind(&Amethyst_GL::fullscn_toggle,this));
  input->sig_kb_ctl[SDL_SCANCODE_3].connect(bind(&Amethyst_GL::stereo_toggle,this));

  // History Buffer
  input->sig_kb[SDL_SCANCODE_C].connect(bind(&Amethyst_GL::state_save, this));
  input->sig_kb[SDL_SCANCODE_V].connect(bind(&Amethyst_GL::state_recall, this));

  setupFramebuffers();
}


Amethyst_GL::~Amethyst_GL()
{
  hud_shutdown();
  delete input;
}


void Amethyst_GL::setupFramebuffers(void)
{
  // Setup Left
  fbleft_.bind();
  
  texleft_.bind();
  texleft_.image2D(0,GL_RGBA, Global.screen_x, Global.screen_y, 0,GL_RGBA, GL_UNSIGNED_BYTE, 0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  rbleft_.bind();
  rbleft_.setStorage(GL_DEPTH_COMPONENT, Global.screen_x, Global.screen_y);
  
  fbleft_.attachDepth(rbleft_);
  fbleft_.attachColor0(texleft_, 0);

  // Setup Right
  fbright_.bind();

  texright_.bind();
  texright_.image2D(0,GL_RGBA, Global.screen_x, Global.screen_y, 0,GL_RGBA, GL_UNSIGNED_BYTE, 0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  
  rbright_.bind();
  rbright_.setStorage(GL_DEPTH_COMPONENT, Global.screen_x, Global.screen_y);

  fbright_.attachDepth(rbright_);
  fbright_.attachColor0(texright_, 0);

  //Setup vertii for final render pass
  static const GLfloat g_quad_vertex_buffer_data[] = {
    -1.0f, -1.0f, 0.0f,
     1.0f, -1.0f, 0.0f,
    -1.0f,  1.0f, 0.0f,
    -1.0f,  1.0f, 0.0f,
     1.0f, -1.0f, 0.0f,
     1.0f,  1.0f, 0.0f,
  };

  vao_.bind();
  vob_.bind();
  vob_.data(sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, 0, 0, 0);

  // Set Framebuffer back to screen
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void Amethyst_GL::main_loop()
{
  Universe &universe = Global.universe;
  uint hist_index = 0;

  if(0) //For history tracking.  Not used any more
  {
    std::vector<cl::Event> wait_queue, new_events;
    size_t num_objects = Global.universe._object_list.size();

    universe.iterate_gpu_tohistory(num_objects, Global.universe._current, hist_index, wait_queue, new_events);
    hist_index++;
  }
  

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
    Object::sptr sol = universe.object_find("Sol");
    Object::sptr earth = universe.object_find("Earth");
    Object::sptr solearthl1 = universe.object_find("S-E L1");
    Object::sptr solearthl2 = universe.object_find("S-E L2");
    if (sol != NULL && earth != NULL && solearthl1 != NULL && solearthl2 != NULL)
    {
      lib::placement_L1(*sol, *earth, *solearthl1);
      lib::placement_L2(*sol, *earth, *solearthl2);
    }

    // Save to history
    if(0)
    {
      if(hist_index < Global.universe._timesteps && !paused)
      {
        std::vector<cl::Event> wait_queue, new_events;
        size_t num_objects = Global.universe._object_list.size();

        //Global.universe.cl_copytogpu();

        Global.universe.iterate_gpu_tohistory(num_objects, Global.universe._current, hist_index, wait_queue, new_events);
        Global.universe.queue_rk4.finish();

        hist_index++;
        if(hist_index%10)
          std::cout << float(hist_index)/float(Global.universe._timesteps) << std::endl;
      }
      else if(!paused)
      {
        pause_toggle();
        std::string log_msg =  "Buffer full.  Simulation Stopped.  Filling distance buffer.";
        Global.log.add(log_msg);
        universe.cl_fill_distance_buff();
        
        log_msg =  "Saving History..";
        Global.log.add(log_msg);
        universe.cl_save_history("dump-halo.h5");

        log_msg =  "Done.";
        Global.log.add(log_msg);
      }
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
  // Update Components Once
  if(show_ui)
    ui.update();
  
  uint maxeyes = (stereo)?2:1;
  //Render each perspective
  for (uint eyeframe = 0; eyeframe < maxeyes; eyeframe++)
  {
      // Bind frame
      if(eyeframe == 0)
        fbleft_.bind();
      else
	fbright_.bind();

      // Clear the window with current clearing color
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
      // Render Scene
      scene_render();

      sig_render_scene(Global.obj_view->location);

      // Show HUD and UI
      if(show_hud)
        hud_render();  

      if(show_ui)
        ui.render();
  }
  
  //Display SBS
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  fbshader_.use();
  vao_.bind();
  
  // Display Left
  fbshader_.Uniform1f(shadereye_, stereo?-1.0f:0.0f);
  texleft_.bind();
  glDrawArrays(GL_TRIANGLES, 0, 6);
  
  if (stereo)
  {
    // Display Right
    fbshader_.Uniform1f(shadereye_, 1.0f);
    texright_.bind();
    glDrawArrays(GL_TRIANGLES, 0, 6);
  }
 
  // Do the buffer Swap
  SDL_GL_SwapWindow(Global.mainwindow);
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


void Amethyst_GL::fullscn_toggle()
{
  if (fullscreen)
  {
    SDL_SetWindowFullscreen(Global.mainwindow, 0);
    SDL_RestoreWindow(Global.mainwindow);
  }
  else
  { 
    //SDL_SetWindowFullscreen(Global.mainwindow, SDL_WINDOW_FULLSCREEN);
    SDL_MaximizeWindow(Global.mainwindow);
    SDL_SetWindowFullscreen(Global.mainwindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
  }

  fullscreen = !fullscreen;
  
  std::cout << "Toggle Fullscreen: " << fullscreen << std::endl;
}


void Amethyst_GL::stereo_toggle()
{
    stereo = !stereo;
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






