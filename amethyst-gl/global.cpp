/***********************************************************************
 Amethyst-GL
  - Instantiation of global class

 Authors (c):
 2006-2008 Beau V.C. Bellamy (beau@stellarnetservices.net)

 $Revision$
 $LastChangedDate$
 $LastChangedBy$
 ***********************************************************************/



#include "global.h"

__global::__global()
{
    cam_num   = 0;
    cam_yaw   = 0.0f;
    cam_pitch = 0.0f;
    cam_zoom  = 20.0f;

    // Field of view (in degrees)
    fov      =     40;

    // Timers
    time_ticks    = 0;
    time_interval = 0;

    joy_null =   5000;
    joy_max  =  32767;
    joy_min  = -32768;

    throttle = 0.1f;

    axis_pitch.joystick    = 0;
    axis_pitch.axis        = 1;
    //axis_pitch.null        = 1000;

    axis_roll.joystick     = 0;
    axis_roll.axis         = 0;
    //axis_roll.null         = 1000;

    axis_yaw.joystick      = 0;
    axis_yaw.axis          = 2;
    //axis_yaw.null          = 1000;

    axis_throttle.joystick = 0;
    axis_throttle.axis     = 3;
    //axis_throttle.null     = 1000;

    file_tex_sun       = "sun2048.bmp";

    file_tex_sb_bk     = "bk.bmp";
    file_tex_sb_ft     = "ft.bmp";
    file_tex_sb_up     = "up.bmp";
    file_tex_sb_dn     = "dn.bmp";
    file_tex_sb_rt     = "rt.bmp";
    file_tex_sb_lt     = "lt.bmp";


    net_server         = "www.stellarnetservices.net";
    net_port           = 23232;
    net_handle         = "leetsuace";

    Global.ship        = &Global.reference_object;
};

__global Global;
