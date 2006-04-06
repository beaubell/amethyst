/***************************************************************************
 *   Instantiation of Global Class                                         *
 *  (c) 2006 Beau V.C. Bellamy (beau@stellarnetservices.net)               *
 ***************************************************************************/

#include "global.h"

__global::__global()
{
    ship.location.x = 10;
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


    //net_server         = "www.stellarnetservices.net";
    net_server         = "localhost";
    net_port           = 23232;

    net_handle         = "Anonymous";
};

__global Global;
