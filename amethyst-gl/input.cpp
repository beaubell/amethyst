/***************************************************************************
 *  Starfighter Input Routines                                             *
 *  (c) 2006 Beau V.C. Bellamy (beau@stellarnetservices.net)               *
 ***************************************************************************/

#include "input.h"

#include "global.h"
#include "debug.h"

#include "lib/utility.h"

#include <math.h>

#define TODEG(x)    x = x * 180.0f / M_PI
#define TORAD(x)    x = x / 180.0f * M_PI

#ifdef WIN32
#define M_PI 3.1415926535897932384626433832795f
#endif

bool mouse_camera = false;

static Cartesian_Vector QVRotate(Quaternion &q, const Cartesian_Vector &v)
{
    Quaternion t;

    t = (q*v)*q.Bar();
    return t.GetVector();
}


int process_inputs()
{
    SDL_Event event;

    /* process pending events */
    while( SDL_PollEvent( &event ) ) {

        switch( event.type ) {
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        return 1;

                    case SDLK_KP_PLUS:
//                    level++;
//                    if (level > 5) level=5;
                        break;

                    case SDLK_KP_MINUS:
//                    level--;
//                    if (level < 0) level=0;
                        break;
                    case SDLK_a:
                    {
                        void *foo = malloc(100000);
                        break;
                    }
                    case SDLK_d:
                        print_object(*Global.ship);
                        break;
                    case SDLK_n:
                        print_object(Global.net_ship[0]);
                        break;
                    case SDLK_b:
                        print_trace();
                        break;
                } break;
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT)
                    mouse_camera = true;
                break;

            case SDL_MOUSEBUTTONUP:
                if (event.button.button == SDL_BUTTON_LEFT)
                    mouse_camera = false;
                else if (event.button.button == SDL_BUTTON_WHEELUP)
                {
                    Global.cam_zoom /= 1.1;
                    if (Global.cam_zoom < 10) Global.cam_zoom = 10;
                }
                else if (event.button.button == SDL_BUTTON_WHEELDOWN)
                    Global.cam_zoom *= 1.1;
                break;

            case SDL_MOUSEMOTION:
                if (mouse_camera)
                {
                    Global.cam_yaw -= ((float)event.motion.xrel)/3;
                    if (Global.cam_yaw < -180) Global.cam_yaw += 360;
                    if (Global.cam_yaw >  180) Global.cam_yaw -= 360;
                    Global.cam_pitch -= ((float)event.motion.yrel)/3;
                    if (Global.cam_pitch < -90) Global.cam_pitch = -90;
                    if (Global.cam_pitch >  90) Global.cam_pitch = 90;
                }
                break;

            case SDL_VIDEORESIZE:
                SDL_SetVideoMode( event.resize.w, event.resize.h, 32, SDL_OPENGL |  SDL_RESIZABLE);
                opengl_change_aspect(event.resize.w, event.resize.h);
                break;

            case SDL_QUIT:
                return 1;
        }
    }


    // Get Global State
    Cartesian_Vector &position = Global.ship->location;
    Cartesian_Vector &velocity = Global.ship->velocity;
    Cartesian_Vector &thrust   = Global.ship->force;
    Cartesian_Vector &accel    = Global.ship->acceleration;
    Cartesian_Vector &force    = Global.ship->force;
    Quaternion       &attitude = Global.ship->attitude;

    unsigned short   &joy_null = Global.joy_null;
    short            &joy_max  = Global.joy_max;
    short            &joy_min  = Global.joy_min;

    Joy_Axis_Map     &joy_pitch    = Global.axis_pitch;
    Joy_Axis_Map     &joy_roll     = Global.axis_roll;
    Joy_Axis_Map     &joy_yaw      = Global.axis_yaw;
    Joy_Axis_Map     &joy_throttle = Global.axis_throttle;



    // Initialize Context Data
    GLfloat yRot     = 0.0f;
    GLfloat xRot     = 0.0f;
    GLfloat zRot     = 0.0f;
    GLfloat throttle = 0.0f;
    accel.clear();
    thrust.clear();

    float scaler = 1.0f / (float(joy_max - joy_null));

    // Get Inputs
    yRot     = float(joystick_axis_norm (SDL_JoystickGetAxis(
            Global.joystick[joy_roll.joystick],     joy_roll.axis),     joy_null) * scaler);
    xRot     = float(joystick_axis_norm (SDL_JoystickGetAxis(
            Global.joystick[joy_pitch.joystick],    joy_pitch.axis),    joy_null) * scaler);
    zRot     = -float(joystick_axis_norm(SDL_JoystickGetAxis(
            Global.joystick[joy_yaw.joystick],      joy_yaw.axis),      joy_null) * scaler);
    throttle = -float(joystick_axis_norm(SDL_JoystickGetAxis(
            Global.joystick[joy_throttle.joystick], joy_throttle.axis), joy_null) * scaler);


    if (SDL_JoystickGetButton(Global.joystick[0], 15)) thrust.x += .001;
    if (SDL_JoystickGetButton(Global.joystick[0], 17)) thrust.x -= .001;

    if (SDL_JoystickGetButton(Global.joystick[0], 14)) thrust.z += .001;
    if (SDL_JoystickGetButton(Global.joystick[0], 16)) thrust.z -= .001;

    if (SDL_JoystickGetButton(Global.joystick[0], 0)) {
        velocity.clear();
    };

    if (SDL_JoystickGetButton(Global.joystick[0], 7)) {
        attitude.w = 1;
        attitude.x = 0;
        attitude.y = 0;
        attitude.z = 0;
        position.clear();
        velocity.clear();
    };


    //Ship Rotation
    TORAD(xRot);
    TORAD(yRot);
    TORAD(zRot);

    Euler eul(xRot, yRot, zRot);
    Quaternion quat(eul);

    attitude *= quat;
    attitude.normalize();


    //Apply throttle state to force vector
    if (throttle != 0) {

        thrust.y = throttle*10000;
        Global.throttle = throttle;
    }

    // Rotate trust vector to match ship orientation
    force = QVRotate(attitude, thrust);


    // Process trajectory information for net ships to make them smoother.
    for (int i = 0; i < Global.net_ships; i++)
    {
        //Global.net_ship[i].velocity += Global.net_ship[i].acceleration;
        //Global.net_ship[i].location += Global.net_ship[i].velocity;
    }

    return 0;

}
