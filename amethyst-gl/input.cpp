/***********************************************************************
 Amethyst-GL
  - Input function definitions

 Authors (c):
 2006-2008 Beau V.C. Bellamy (beau@stellarnetservices.net)

 $Revision$
 $LastChangedDate$
 $LastChangedBy$
 ***********************************************************************/

#include <iostream>

#include "input.h"
#include "global.h"
#include "debug.h"
#include "scene.h"
#include "scene_xml.h"

#include "lib/utility.h"

#include <boost/lexical_cast.hpp>
#include <math.h>

namespace amethyst {
namespace client {


Input::Input(Amethyst_GL &context)
    : kb_lalt(false),
      kb_ralt(false),
      kb_lctrl(false),
      kb_rctrl(false),
      kb_lshift(false),
      kb_rshift(false),
      mouse_camera(false),
      ui_has_focus_(false),
      context_(context)
{
}


int Input::process_events()
{
    SDL_Event event;

    /* process pending events */
    while (SDL_PollEvent(&event))
    {

        switch (event.type)
        {
            case SDL_KEYDOWN:
                if(event_keydown(event.key))
                    return 1;
                break;

            case SDL_KEYUP:
                if(event_keyup(event.key))
                    return 1;
                break;

            case SDL_MOUSEBUTTONDOWN:
                event_mouse_buttondown(event.button);
                break;

            case SDL_MOUSEBUTTONUP:
                event_mouse_buttonup(event.button);
                break;

            case SDL_MOUSEMOTION:
                event_mouse_motion(event.motion);
                break;

            case SDL_JOYAXISMOTION:
                event_joy_axismotion(event.jaxis);
                break;

            case SDL_VIDEORESIZE:
                event_video_resize(event.resize);
                break;

            case SDL_QUIT:
                return 1;
        }
    }

    return 0;

}


int Input::event_keydown(const SDL_KeyboardEvent &key)
{
    switch (key.keysym.sym)
    {
        case SDLK_ESCAPE:
            return 1;

        case SDLK_LALT:
            kb_lalt = true;
            break;

        case SDLK_RALT:
            kb_ralt = true;
            break;

        case SDLK_LCTRL:
            kb_lctrl = true;
            break;

        case SDLK_RCTRL:
            kb_rctrl = true;
            break;

        case SDLK_LSHIFT:
            kb_lshift = true;
            break;

        case SDLK_RSHIFT:
            kb_rshift = true;
            break;

        case SDLK_KP_PLUS:
//          level++;
//          if (level > 5) level=5;
            break;

        case SDLK_KP_MINUS:
//          level--;
//          if (level < 0) level=0;
            break;
        case SDLK_SPACE:
        {
            sig_kb_space();
            break;
        }
        case SDLK_a:
        {
            /// Ctrl A
            if(kb_lctrl || kb_rctrl)
            {
                std::string log = "ALT CTRL Works!";
                Global.log.add(log);
            }
            else
              sig_kb_a();
            break;
        }
        case SDLK_b:
            print_trace();
            break;

        case SDLK_c:
            scene_control_ship_next();
            break;

        case SDLK_d:
            //scene_xml_write("Dump");
            sig_kb_d();
            break;

        case SDLK_e:
            //scene_select_object_next();
            sig_kb_e();
            break;

        case SDLK_f:
        {
            //if (glWindowPosSupported)
            //    if (glWindowPosEnabled)
            //        glWindowPosEnabled = false;
            //    else
            //        glWindowPosEnabled = true;
            scene_select_object_next();
            break;
        }

        case SDLK_g: // Toggle gravity on and off
            Global.universe.do_gravity_calc = !Global.universe.do_gravity_calc;
            break;

        case SDLK_n:
            scene_target_object_next();
            break;

        case SDLK_p:
            //print_object(Global.net_ship[0]);
            break;

        case SDLK_q:
        {
            sig_kb_q();
            break;
        }
        case SDLK_s: // Toggle shaders on and off
        {
            //if (glShaderObjectsSupported)
            //    if (glShaderObjectsEnabled)
            //    {
            //        glShaderObjectsEnabled = false;
            //        glUseProgramObjectARB(0);
            //    }
            //    else
            //    {
            //        glShaderObjectsEnabled = true;
            //        glUseProgramObjectARB(Global.shaderProgram);
            //    }
            sig_kb_s();
            break;
        }
        case SDLK_w:
        {
            sig_kb_w();
            break;
        }
        case SDLK_z:
        {
            sig_kb_z();
            break;
        }
        default:
             std::string log = "Unhandled keystroke: " + boost::lexical_cast<std::string>(key.keysym.sym);
             Global.log.add(log);
             break;
    } // switch

    return 0;

}


int Input::event_keyup(const SDL_KeyboardEvent &key)
{
    switch (key.keysym.sym)
    {
        case SDLK_LALT:
            kb_lalt = false;
            break;

        case SDLK_RALT:
            kb_ralt = false;
            break;

        case SDLK_LCTRL:
            kb_lctrl = false;
            break;

        case SDLK_RCTRL:
            kb_rctrl = false;
            break;

        case SDLK_LSHIFT:
            kb_lshift = false;
            break;

        case SDLK_RSHIFT:
            kb_rshift = false;
            break;

        default:
            break;
    }

    return 0;
}


int Input::event_mouse_motion(const SDL_MouseMotionEvent &motion)
{
    if (mouse_camera)
    {
        Global.cam_yaw -= static_cast<float>(motion.xrel) / 3;
        if (Global.cam_yaw < -180) Global.cam_yaw += 360;
        if (Global.cam_yaw >  180) Global.cam_yaw -= 360;
        Global.cam_pitch -= static_cast<float>(motion.yrel) / 3;
        if (Global.cam_pitch < -90) Global.cam_pitch = -90;
        if (Global.cam_pitch >  90) Global.cam_pitch = 90;
    }

    return 0;
}


int Input::event_mouse_buttondown(const SDL_MouseButtonEvent &button)
{
    if(context_.ui.check_focus(button.x,button.y, button.button))
    {
       ui_has_focus_ = true;
       return 0;
    }
    if (button.button == SDL_BUTTON_LEFT)
    {
        //// Check to see if UI wants to claim that button click
        //if(context_.ui.check_focus(button.x,button.y, button.))
	//{
	// ui_has_focus_ = true;
	//}
	//else
	{
	  ui_has_focus_ = false;
	  mouse_camera = true;
	}
    }

    return 0;
}


int Input::event_mouse_buttonup(const SDL_MouseButtonEvent &button)
{
    if(context_.ui.check_focus(button.x,button.y, button.button))
    {
       ui_has_focus_ = true;
       return 0;
    }
    
    if (button.button == SDL_BUTTON_LEFT)
        mouse_camera = false;
    else if (button.button == SDL_BUTTON_WHEELUP)
    {
        Global.cam_zoom /= 1.1f;
        if (Global.cam_zoom < 10) Global.cam_zoom = 10;
    }
    else if (button.button == SDL_BUTTON_WHEELDOWN)
        Global.cam_zoom *= 1.1f;

    return 0;
}


int Input::event_joy_axismotion(const SDL_JoyAxisEvent &jaxis)
{
    unsigned char  js    = jaxis.which;
    unsigned char  axis  = jaxis.axis;
    signed   short value = jaxis.value;

    float norm_value = 0.0f;

    if (value < -5000)
        norm_value = static_cast<float>(value+5000) / 32768.0f;

    if (value > 5000)
        norm_value = static_cast<float>(value-5000) / 32768.0f;

    if(axis == Global.axis_pitch.axis)
    {
        std::cout << "Pitch Norm: " << boost::lexical_cast<std::string>(norm_value) << std::endl;
        if (Global.ship)
            Global.ship->set_att_thrust_pitch(norm_value);
    }
    else if (axis == Global.axis_roll.axis)
    {
        if (Global.ship)
            Global.ship->set_att_thrust_roll(norm_value);
    }
    else if(axis == Global.axis_yaw.axis)
    {
        if (Global.ship)
            Global.ship->set_att_thrust_yaw(-norm_value);
    }
    else if(axis == Global.axis_throttle.axis)
    {
        if (Global.ship)
            Global.ship->set_throttle(-norm_value);
    }

	return 0;
}

int Input::event_video_resize(const SDL_ResizeEvent &resize)
{
    SDL_SetVideoMode(resize.w, resize.h, 32, SDL_OPENGL |  SDL_RESIZABLE);

    opengl_change_aspect(resize.w, resize.h);

    return 0;
}


} // namespace client
} // namespace amethyst
