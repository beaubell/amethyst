/***********************************************************************
 Amethyst-GL
  - Input function definitions

 Authors (c):
 2006-2020 Beau V.C. Bellamy (bellamy.beau@gmail.com)
 ***********************************************************************/

#include <iostream>

#include "input.h"
#include "global.h"
#include "debug.h"

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
                
            case SDL_MOUSEWHEEL:
                event_mouse_wheel(event.wheel);
                break;

            case SDL_JOYAXISMOTION:
                event_joy_axismotion(event.jaxis);
                break;

            case SDL_WINDOWEVENT:
                event_window(event.window);
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

        default:
        {
          SDL_Scancode scancode = SDL_GetScancodeFromKey(key.keysym.sym);
          
          if (kb_lctrl || kb_rctrl)
            sig_kb_ctl[scancode]();
          else if (kb_lshift || kb_rshift)
            sig_kb_shift[scancode]();
          else
          {
            if(sig_kb[scancode].num_slots() != 0)
            {
	      std::cout << "number of slots: " << sig_kb[scancode].num_slots() << std::endl;
              sig_kb[scancode]();
            }
            else
            {
              std::string log = "keystroke, " + boost::lexical_cast<std::string>(scancode) + ", is not connected to an operation.";
              Global.log.add(log);
            }
          }
          break;
        }
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
        Camera &camera = context_.get_scene().get_camera();
        camera.incYaw(-static_cast<double>(motion.xrel) / 3.0);
        camera.incPitch(-static_cast<double>(motion.yrel) / 3.0);
    }

    return 0;
}

int Input::event_mouse_wheel(const SDL_MouseWheelEvent &wheel)
{
    Camera &camera = context_.get_scene().get_camera();
    auto dist = camera.getDistance();
    auto newdist = dist / pow(1.1,wheel.y);

    if (newdist < 10) newdist = 10;

    camera.setDistance(newdist);
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
          SDL_SetRelativeMouseMode(SDL_TRUE);
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
    {
        mouse_camera = false;
        SDL_SetRelativeMouseMode(SDL_FALSE);
    }
    
    return 0;
}


int Input::event_joy_axismotion(const SDL_JoyAxisEvent &jaxis)
{
    /* unsigned char  js    = jaxis.which;   UNUSED */
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

int Input::event_window(const SDL_WindowEvent &window)
{
    if (window.event == SDL_WINDOWEVENT_RESIZED)
    {
        int w = window.data1;
        int h = window.data2;
        //SDL_SetVideoMode(w, h, 32, SDL_OPENGL |  SDL_RESIZABLE);

        opengl_change_aspect(w, h);
	context_.setupFramebuffers();
    }
    return 0;
}


} // namespace client
} // namespace amethyst
