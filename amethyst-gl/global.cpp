/***********************************************************************
 Amethyst-GL
  - Instantiation of global class

 Authors (c):
 2006-2023 Beau V.C. Bellamy (bellamy.beau@gmail.com)
 ***********************************************************************/

#include "global.h"

namespace amethyst::client {

__global::__global()
{
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


    net_server         = "athena.bvcb.us";
    net_port           = 23232;
    net_handle         = "leetsuace";

    reference_object   = std::make_shared<Scene_Object>();
    reference_ship     = std::make_shared<Scene_Ship>();

    ship               = Global.reference_ship;
    obj_target         = Global.reference_object;
    obj_view           = Global.reference_object;

    reference_ship->name = "Nothing";
};

__global Global;

} // namespace amethyst::client
