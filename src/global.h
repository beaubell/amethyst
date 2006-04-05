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

#include "joystick.h"

using namespace amethyst;


class __global {

   public:
      __global() { ship.location.x = 10;
                   ship.location.y = 0;
                   ship.location.z = 0;

                   ship.attitude.w = 1;
                   ship.attitude.x = 0;
                   ship.attitude.y = 0;
                   ship.attitude.z = 0;

                   cam_num = 0;

                   // Field of view (in degrees)
                   fov      =     40;

                   joy_null =   1000;
                   joy_max  =  32767;
                   joy_min  = -32768;

                   axis_pitch.joystick    = 0;
                   axis_pitch.axis        = 1;
                   //axis_pitch.null        = 1000;

                   axis_roll.joystick     = 0;
                   axis_roll.axis         = 0;
                   //axis_roll.null         = 1000;

                   axis_yaw.joystick      = 0;
                   axis_yaw.axis          = 3;
                   //axis_yaw.null          = 1000;

                   axis_throttle.joystick = 0;
                   axis_throttle.axis     = 4;
                   //axis_throttle.null     = 1000;

                   dir_home           = "/home/beau";
                   dir_amethyst       = dir_home + "/.amethyst";
                   dir_textures       = dir_amethyst + "/textures";

                   file_tex_planet    = dir_textures + "/mars2048.bmp";
                   file_tex_starfield = dir_textures + "/space2048.bmp";

                   file_tex_sb_bk     = dir_textures + "/bk.bmp";
                   file_tex_sb_ft     = dir_textures + "/ft.bmp";
                   file_tex_sb_up     = dir_textures + "/up.bmp";
                   file_tex_sb_dn     = dir_textures + "/dn.bmp";
                   file_tex_sb_rt     = dir_textures + "/rt.bmp";
                   file_tex_sb_lt     = dir_textures + "/lt.bmp";
                 };

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

      std::string dir_home;
      std::string dir_amethyst;
      std::string dir_textures;

      std::string file_tex_planet;
      std::string file_tex_starfield;

      std::string file_tex_sb_bk;
      std::string file_tex_sb_ft;
      std::string file_tex_sb_up;
      std::string file_tex_sb_dn;
      std::string file_tex_sb_rt;
      std::string file_tex_sb_lt;

};

// Tell all c files that Global is instantiated somewhere externally
extern __global Global;


#endif /* GLOBAL_H */
