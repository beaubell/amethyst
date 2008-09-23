#ifndef GLOBAL_H
#define GLOBAL_H

/***********************************************************************
 Amethyst-GL
  - Global parameters class definition and default values

 Authors (c):
 2006-2008 Beau V.C. Bellamy (beau@stellarnetservices.net)

 $Revision$
 $LastChangedDate$
 $LastChangedBy$
 ***********************************************************************/

#include "lib/object.h"

#include <string>
#include "opengl.h"
#include "SDL_net.h"

#include "joystick.h"

#define WIDTH  800
#define HEIGHT 600

#ifdef WIN32
#define M_PI 3.14159265358979323846
#endif

#define TODEG(x)    x = x * 180.0 / M_PI
#define TORAD(x)    x = x / 180.0 * M_PI

using namespace amethyst;

class __global {

   public:
      __global (void);
      std::string scene;

      // Your Ship
      Object *ship;

      // Models // FIXME
      GLuint sun_mdl;


      // Textures // FIXME
      GLuint sun_tex;


      // Field of View
      float   fov;

      // Joystick
      int               joysticks;         // Number of joysticks
      SDL_Joystick     *joystick[16];      // Pointers to joysticks
      unsigned short    joy_null;          // Null zone for joystick axeses
      short             joy_max;           // Max value for joystick axeses
      short             joy_min;           // Min value for joystick axeses
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

      // Paths
      std::string dir_home;
      std::string dir_amethyst;
      std::string dir_scene;
      std::string dir_textures;
      std::string dir_models;
      std::string dir_fonts;

      // Texture File Paths
      std::string file_tex_sun;

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

      IPaddress   net_server_ip;
      UDPsocket   net_socket;
      UDPpacket   *pack_in, *pack_out;

      // Network ships
      Object net_ship[5];
      int    net_ships;


};

// Tell all c files that Global is instantiated somewhere externally
extern __global Global;


#endif /* GLOBAL_H */
