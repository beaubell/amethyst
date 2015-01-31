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

namespace amethyst {
namespace client {

__global::__global()
{
    cam_num   = 0;
    cam_yaw   = 0.0f;
    cam_pitch = 0.0f;
    cam_zoom  = 2.0e8f;

    // Field of view (in degrees)
    fov      =     40;

    mainwindow = NULL;
    maincontext = 0;

    screen_x = 1024;
    screen_y = 640;

    // Timers
    time_ticks    = 0;
    time_interval = 0;

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

    file_tex_sb_bk     = "bk.bmp";
    file_tex_sb_ft     = "ft.bmp";
    file_tex_sb_up     = "up.bmp";
    file_tex_sb_dn     = "dn.bmp";
    file_tex_sb_rt     = "rt.bmp";
    file_tex_sb_lt     = "lt.bmp";


    net_server         = "www.stellarnetservices.net";
    net_port           = 23232;
    net_handle         = "leetsuace";

    ship               = &Global.reference_ship;
    obj_target         = &Global.reference_object;
    obj_view           = &Global.reference_object;

    reference_ship.name = "Nothing";
};

__global Global;

} // namespace client
} // namespace amethyst
