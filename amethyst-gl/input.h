#ifndef AMETHYST_CLIENT_INPUT_H
#define AMETHYST_CLIENT_INPUT_H

/***********************************************************************
 Amethyst-GL
  - Input function declarations

 Authors (c):
 2006-2008 Beau V.C. Bellamy (beau@stellarnetservices.net)

 $Revision$
 $LastChangedDate$
 $LastChangedBy$
 ***********************************************************************/

// Forward Delcaration
namespace amethyst {
namespace client {
  class Input;
}
}

#include <boost/signals2.hpp>

#include "joystick.h"
#include "amethyst-gl.h"

namespace amethyst {
namespace client {


class Input
{
   public:
    Input(Amethyst_GL &context);

    int process_events(void);

    Joystick js;
    typedef boost::signals2::signal<void ()> Signal_kb;
    typedef boost::signals2::signal<void (signed short)> Signal_axis;

    Signal_kb sig_kb[SDL_NUM_SCANCODES];
    Signal_kb sig_kb_shift[SDL_NUM_SCANCODES];
    Signal_kb sig_kb_ctl[SDL_NUM_SCANCODES];

   private:
    /// Process Keyboard Key Down/Up
    int event_keydown(const SDL_KeyboardEvent &key);
    int event_keyup(const SDL_KeyboardEvent &key);

    /// Process Mouse Motion
    int event_mouse_motion(const SDL_MouseMotionEvent &motion);
    int event_mouse_wheel(const SDL_MouseWheelEvent &wheel);

    /// Process Mouse Button Down/Up
    int event_mouse_buttondown(const SDL_MouseButtonEvent &button);
    int event_mouse_buttonup(const SDL_MouseButtonEvent &button);

    /// Process Joystick Axis Motion
    int event_joy_axismotion(const SDL_JoyAxisEvent &jaxis);

    /// Process Joystick Ball Motion
    int event_joy_ballmotion(const SDL_JoyBallEvent &jball);

    /// Process Joystick Hat Motion
    int event_joy_hatmotion(const SDL_JoyHatEvent &jhat);

    /// Process Joystick Button Down/Up
    int event_joy_buttondown(const SDL_JoyButtonEvent &jbutton);
    int event_joy_buttonup(const SDL_JoyButtonEvent &jbutton);

    /// Process Video Resize
    int event_window(const SDL_WindowEvent &window);

    /// Process Quit
    int event_quit(const SDL_QuitEvent &quit);

    /// Process User Event
    int event_user(const SDL_UserEvent &user);

    /// Process Window Manager Events
    int event_wm(const SDL_SysWMEvent &syswm);

    /// flag for when alt key is held down.
    bool kb_lalt;
    bool kb_ralt;

    /// flag for when ctrl key is held down.
    bool kb_lctrl;
    bool kb_rctrl;

    /// flag for when shift key is held down.
    bool kb_lshift;
    bool kb_rshift;

    /// Flag for when mouse movement is controlling the camera.
    bool mouse_camera;
    
    /// flag for when UI has input focus
    bool ui_has_focus_;
    
    Amethyst_GL &context_;

};


} // namespace client
} // namespace amethyst

#endif /* AMETHYST_CLIENT_INPUT_H */
