/***************************************************************************
 *  Starfighter Main/Setup functions                                      *
 *  (c) 2006 Beau V.C. Bellamy (beau@stellarnetservices.net)               *
 ***************************************************************************/


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>
#include <cstdlib>

#include <math.h>

#include <amethyst/vector.h>
#include <amethyst/orientation.h>
#include <amethyst/object.h>
#include <amethyst/physics.h>
#include <amethyst/utility.h>
#include <amethyst/universe.h>

#include <stdlib.h>

#include "global.h"
#include "opengl.h"
#include "joystick.h"
#include "net.h"
#include "texture.h"
#include "model.h"
#include "config_xml.h"
#include "stars.h"
#include "thread.h"
#include "scene.h"
#include "timer.h"
#include "hud.h"
#include "debug.h"
#include "file.h"

#define TICK_INTERVAL    10

#ifdef WIN32
#define M_PI 3.1415926535897932384626433832795f
#define PROFILE_DIR_ENV "APPDATA"
#define PROJECT_DIR "Amethyst"
#else
#define PROFILE_DIR_ENV "HOME"
#define PROJECT_DIR ".amethyst"
#endif

#define TODEG(x)    x = x * 180.0f / M_PI
#define TORAD(x)    x = x / 180.0f * M_PI

using namespace std;
using namespace amethyst;

static void setup_sdl(void);

// Create universe
Universe universe;

static Cartesian_Vector QVRotate(Quaternion &q, const Cartesian_Vector &v)
{
  Quaternion t;

  t = (q*v)*q.Bar();
  return t.GetVector();
}


void setup_objects(void)
{
    Object *temp;

    // Player Ship
    temp = new amethyst::Object;
    temp->name     = "[ ]" + Global.net_handle;
    temp->mass     = 100.0f;
    temp->location = Cartesian_Vector(2.6479986873564962E+02,  2.5088154658693099E+02, 8.8188716221901501E+01);
    temp->velocity = Cartesian_Vector(-2.7992207996826437E+01, 9.1437922883469778E+00, -2.4572578177548952E+01);
    temp->attitude = Quaternion(0.364095,0.256614,-0.265440,0.855059);
    temp->meta = (void*)Global.dlShip;
    scene_add_object(temp);
    universe.object_add(temp);
    Global.ship = temp;

    // Planet
    temp = new amethyst::Object;
    temp->name     = "-Mars";
    temp->mass     = 10000000000000000.0f;
    temp->location = amethyst::Cartesian_Vector(100,100,100);
    //temp->velocity = amethyst::Cartesian_Vector(10,0,0);
    temp->meta = (void*)Global.planet_mdl;
    scene_add_object(temp);
    universe.object_add(temp);

    // Static Ship
    temp = new amethyst::Object;
    temp->name     = "-Ship";
    temp->location = Cartesian_Vector(0, 40, 3);
    temp->meta = (void*)Global.dlShip;
    scene_add_object(temp);

}


static void setup_sdl()
{
    const SDL_VideoInfo* video;

    cout << "Initializing SDL...";
    if ( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
        fprintf(stderr,
                "Couldn't initialize SDL: %s\n", SDL_GetError());
        exit(1);
    } else cout << "Success!" << endl;

    atexit(SDL_Quit);

	SDL_WM_SetCaption("l33t Starfightar Gaem!  Biatch!","Starfighter");

    cout << "Initializing Video...";
    video = SDL_GetVideoInfo( );

    if( !video ) {
        fprintf(stderr,
                "Couldn't get video information: %s\n", SDL_GetError());
        exit(1);
    } else cout << "Success!" << endl;

    cout << "Initializing OpenGL (Part 1)...";

    /* Set the minimum requirements for the OpenGL window */
    SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

    if( SDL_SetVideoMode( WIDTH, HEIGHT, video->vfmt->BitsPerPixel, SDL_OPENGL |  SDL_RESIZABLE) == 0 ) {
        fprintf(stderr,
                "Couldn't set video mode: %s\n", SDL_GetError());
        exit(1);
    } else cout << "Sucess!" << endl;

}


static void process_inputs()
{
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
        }

    // Rotate trust vector to match ship orientation
    force = QVRotate(attitude, thrust);


    // Process trajectory information for net ships to make them smoother.
    for (int i = 0; i < Global.net_ships; i++)
    {
        //Global.net_ship[i].velocity += Global.net_ship[i].acceleration;
        //Global.net_ship[i].location += Global.net_ship[i].velocity;
    }

}



static void main_loop()
{
    SDL_Event event;
    Global.next_time = SDL_GetTicks() + TICK_INTERVAL;

    while(1) {
        /* process pending events */
        while( SDL_PollEvent( &event ) ) {

            switch( event.type ) {
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    return;

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
            case SDL_MOUSEMOTION:
                Global.cam_yaw -= event.motion.xrel;
                if (Global.cam_yaw < -180) Global.cam_yaw += 360;
                if (Global.cam_yaw >  180) Global.cam_yaw -= 360;
                Global.cam_pitch -= event.motion.yrel;
                if (Global.cam_pitch < -90) Global.cam_pitch = -90;
                if (Global.cam_pitch >  90) Global.cam_pitch = 90;

                break;

            case SDL_VIDEORESIZE:
                  SDL_SetVideoMode( event.resize.w, event.resize.h, 32, SDL_OPENGL |  SDL_RESIZABLE);
                  opengl_change_aspect(event.resize.w, event.resize.h);
                break;

             case SDL_QUIT:
               return;
            }
        }

    // Process Inputs
    process_inputs();

    // Iterate Physics Engine
    Global.time_interval = SDL_GetTicks() - Global.time_ticks;
    Global.time_ticks += Global.time_interval;
    if (Global.time_interval > 0)
        universe.iterate(Global.time_interval / 1000.0);

    /* update the screen */
    RenderScene();
    //SDL_Delay(time_left());
    //Global.next_time += TICK_INTERVAL;
    }
}


int main(int argc, char* argv[])
{
	//Global.dir_home     = "Z:";

    Global.dir_home     = getenv (PROFILE_DIR_ENV);
    Global.dir_amethyst = Global.dir_home + "/" + PROJECT_DIR;
    Global.dir_textures = Global.dir_amethyst + "/" + "textures";
    Global.dir_models   = Global.dir_amethyst + "/" + "models";
    Global.dir_fonts    = Global.dir_amethyst + "/" + "fonts";

    string config_file  = Global.dir_amethyst + "/" + "config.xml";

    string stars_file   = Global.dir_amethyst + "/" + "stars.csv";


    // Check for existance of config.xml else fail siliently
    if(access(config_file.c_str(), F_OK) == 0) {

        // Check for read permission else fail kicking and screaming
        if(access(config_file.c_str(), R_OK) == 0) {
            parse_xml_config (config_file.c_str());
            printf("Reading config file at %s...\n", config_file.c_str());
        } else {
            printf("Unable to read config file at %s, permission?\n", config_file.c_str());
        }
    }



    setup_sdl();

    setup_opengl();

    setup_joystick();

    setup_network();

    setup_hud();


    load_stars(stars_file);
    load_models();

    setup_objects();

      //load_skybox();

      main_loop();

    free_models();
    free_stars();

    thread_stop_all();

    SDL_Quit ();
    return EXIT_SUCCESS;
}
