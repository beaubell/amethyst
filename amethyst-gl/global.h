#pragma once
/***********************************************************************
 Amethyst-GL
  - Global parameters class definition and default values

 Authors (c):
 2006-2023 Beau V.C. Bellamy (bellamy.beau@gmail.com)
 ***********************************************************************/

#include "scene_object.h"
#include "lib/universe.h"
#include "camera.h"

#include <string>
#include <set>

#include "joystick.h"
#include "log.h"

#define WIDTH  800
#define HEIGHT 600

#ifdef WIN32
#define M_PI 3.14159265358979323846
#endif

namespace amethyst::client {

using lib::Universe;
using lib::Quaternion;

class __global {

   public:
      __global (void);

      // Physics Engine
      Universe universe;

      std::string scene_name;

      // Your Ship
      lib::Ship::sptr ship;

      // Targeted Object
      lib::Object::sptr obj_target;

      // Viewed Object
      lib::Object::sptr obj_view;

      // Field of View
      float   fov;

      // Video Information
      SDL_Window*    mainwindow;
      SDL_GLContext  maincontext;

      unsigned int screen_x;
      unsigned int screen_y;

      // Joystick
      SDL_Joystick     *joystick[16];      // Pointers to joysticks
      Joy_Axis_Map      axis_pitch;
      Joy_Axis_Map      axis_roll;
      Joy_Axis_Map      axis_yaw;
      Joy_Axis_Map      axis_throttle;

      // Control Surfaces
      float throttle;

      // Timer
      Uint32 next_time;
      Uint32 time_ticks;
      Uint32 time_interval;

      // Paths
      std::string dir_home;
      std::string dir_amethyst;
      std::string dir_scene;
      std::string dir_textures;
      std::string dir_models;
      std::string dir_fonts;
      std::string dir_shaders;
      std::string dir_modules;

      // Network
      std::string net_server;
      short       net_port;

      std::string net_handle;

      // Network ships
      //Scene_Object net_ship[5];
      //int          net_ships;

      // Reference Objects
      Scene_Object::sptr reference_object;
      Scene_Ship::sptr   reference_ship;

      Log    log;

      std::set<Scene_Ship::sptr> ships;
};

// Tell all c files that Global is instantiated somewhere externally
extern __global Global;

} // namespace amethyst::client
