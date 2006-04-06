#ifndef GLOBAL_H
#define GLOBAL_H

/***************************************************************************
 *   Global parameters class definition and default values                 *
 *  (c) 2006 Beau V.C. Bellamy (beau@stellarnetservices.net)               *
 ***************************************************************************/

#include <amethyst/object.h>

#include <string>
#include "SDL/SDL.h"
#include "SDL/SDL_opengl.h"
#include "SDL/SDL_net.h"

#include "joystick.h"

using namespace amethyst;


class __global {

   public:
      __global (void);

      // Your Ship
      Object ship;

      // Model for ship
      GLuint dlShip;          // Display list identifier for the ship
      GLuint planet_mdl;
      GLuint starfield_mdl;

      // Textures
      //GLuint  texture[1];
      GLuint planet_tex;
      GLuint starfield_tex;

      // Pointer to quadratic
      GLUquadric *quadratic;

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

      // Timer
      Uint32 next_time;

      // Paths
      std::string dir_home;
      std::string dir_amethyst;
      std::string dir_textures;

      // Texture File Paths
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


};

// Tell all c files that Global is instantiated somewhere externally
extern __global Global;


#endif /* GLOBAL_H */
