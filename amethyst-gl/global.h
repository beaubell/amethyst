#ifndef AMETHYST_CLIENT_GLOBAL_H
#define AMETHYST_CLIENT_GLOBAL_H

/***********************************************************************
 Amethyst-GL
  - Global parameters class definition and default values

 Authors (c):
 2006-2008 Beau V.C. Bellamy (beau@stellarnetservices.net)

 $Revision$
 $LastChangedDate$
 $LastChangedBy$
 ***********************************************************************/

#include "scene_object.h"
//#include "lib/ship.h"
#include "lib/universe.h"

#include <string>
#include <set>
//#include "opengl.h"

#include "joystick.h"
#include "log.h"

#define WIDTH  800
#define HEIGHT 600

#ifdef WIN32
#define M_PI 3.14159265358979323846
#endif

#define TODEG(x)    x = x * 180.0 / M_PI
#define TORAD(x)    x = x / 180.0 * M_PI

namespace amethyst {
namespace client {

//using lib::Object;
//using lib::Ship;
//using lib::Ship_ptr;
using lib::Universe;
using lib::Quaternion;

class __global {

   public:
      __global (void);

      // Physics Engine
      Universe universe;

      std::string scene;

      // Your Ship
      lib::Ship *ship;

      // Targeted Object
      lib::Object *obj_target;

      // Viewed Object
      lib::Object *obj_view;

      // Field of View
      float   fov;

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

      // Camera Ring Buffer
      Quaternion cam_att[10];
      int cam_num;
      float cam_yaw;
      float cam_pitch;
      float cam_zoom;

      // Timer
      Uint32 next_time;
      Uint32 time_ticks;
      Uint32 time_interval;
      double simulation_time;

      // Paths
      std::string dir_home;
      std::string dir_amethyst;
      std::string dir_scene;
      std::string dir_textures;
      std::string dir_models;
      std::string dir_fonts;
      std::string dir_shaders;
      std::string dir_modules;

      // Texture File Paths - not used anymore
      std::string file_tex_sb_bk;
      std::string file_tex_sb_ft;
      std::string file_tex_sb_up;
      std::string file_tex_sb_dn;
      std::string file_tex_sb_rt;
      std::string file_tex_sb_lt;

      // Network
      std::string net_server;
      short       net_port;

      std::string net_handle;
/*
      IPaddress   net_server_ip;
      UDPsocket   net_socket;
      UDPpacket   *pack_in, *pack_out;
*/
      // Network ships
      //Scene_Object net_ship[5];
      //int          net_ships;

      // Reference Objects
      Scene_Object reference_object;
      Scene_Ship   reference_ship;

      Log    log;

      std::set<Scene_Ship::ptr> ships;
};

// Tell all c files that Global is instantiated somewhere externally
extern __global Global;

} // namespace client
} // namespace amethyst

#endif /* AMETHYST_CLIENT_GLOBAL_H */
