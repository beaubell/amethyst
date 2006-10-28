#ifndef GLOBAL_H
#define GLOBAL_H

/***************************************************************************
 *   Global parameters class definition and default values                 *
 *  (c) 2006 Beau V.C. Bellamy (beau@stellarnetservices.net)               *
 ***************************************************************************/

#include <amethyst/object.h>

#include <string>
#include "opengl.h"
#include "SDL_net.h"

#include "joystick.h"

#define WIDTH  800
#define HEIGHT 600

using namespace amethyst;

class __global {

   public:
      __global (void);

      // Your Ship
      Object *ship;

      // Model for ship
      GLuint dlShip;          // Display list identifier for the ship
      GLuint sun_mdl;
      GLuint planet_mdl;
      GLuint starfield_mdl;

      // Textures
      //GLuint  texture[1];
      GLuint sun_tex;
      GLuint planet_tex;
      GLuint starfield_tex;

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


      // Camera Ring Buffer
      Cartesian_Vector cam_pos[10], cam_view[10], cam_up[10];
      int cam_num;
      float cam_yaw;
      float cam_pitch;

      // Timer
      Uint32 next_time;
      Uint32 time_ticks;
      Uint32 time_interval;

      // Paths
      std::string dir_home;
      std::string dir_amethyst;
      std::string dir_textures;
      std::string dir_models;
      std::string dir_fonts;

      // Texture File Paths
      std::string file_tex_sun;
      std::string file_tex_planet;
      std::string file_tex_starfield;

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
