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

#include "file.h"
#include "input.h"

#define TICK_INTERVAL    10

#ifdef WIN32
#define PROFILE_DIR_ENV "APPDATA"
#define PROJECT_DIR "Amethyst"
#else
#define PROFILE_DIR_ENV "HOME"
#define PROJECT_DIR ".amethyst"
#endif

using namespace std;
using namespace amethyst;

static void setup_sdl(void);

// Create universe
Universe universe;


void setup_objects(void)
{
    Object *temp;

    // Player Ship
    temp = new amethyst::Object;
    temp->name     = "[ ]" + Global.net_handle;
    temp->mass     = 100.0f;
    //temp->location = Cartesian_Vector(2.6479986873564962E+02,  2.5088154658693099E+02, 8.8188716221901501E+01);
    //temp->velocity = Cartesian_Vector(-2.7992207996826437E+01, 9.1437922883469778E+00, -2.4572578177548952E+01);
    temp->attitude = Quaternion(0.000133,0.004793,0.999970,0.006136);
    temp->meta = (void*)Global.dlShip;
    scene_add_object(temp);
    universe.object_add(temp);
    Global.ship = temp;

    // Planet
    temp = new amethyst::Object;
    temp->name     = "-Mars";
    temp->mass     = 6.4185e23;
    temp->location = amethyst::Cartesian_Vector(100,100,7000000);
    //temp->velocity = amethyst::Cartesian_Vector(10,0,0);
    temp->meta = (void*)Global.planet_mdl;
    scene_add_object(temp);
    universe.object_add(temp);

    // Static Ship
    temp = new amethyst::Object;
    temp->name     = "-Ship";
    temp->location = Cartesian_Vector(0, 40, 3);
    temp->attitude = Quaternion(0.000133,0.004793,0.999970,0.006136);
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


static void main_loop()
{
    while(1) {
    Global.next_time = SDL_GetTicks() + TICK_INTERVAL;

    // Process Inputs
    int status = process_inputs();
    if (status) return;

    // Iterate Physics Engine
    Global.time_interval = SDL_GetTicks() - Global.time_ticks;
    Global.time_ticks += Global.time_interval;
    if (Global.time_interval > 0)
        universe.iterate(Global.time_interval / 1000.0);

    /* update the screen */
    RenderScene();
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
