/***********************************************************************
 Amethyst-GL
  - Starup / Main Loop Functions

 Authors (c):
 2006-2008 Beau V.C. Bellamy (beau@stellarnetservices.net)

 $Revision$
 $LastChangedDate$
 $LastChangedBy$
 ***********************************************************************/


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>
#include <cstdlib>
#include <stdexcept>

#include "amethyst-gl.h"
#include "global.h"
#include "opengl.h"
#include "opengl_shader.h"
#include "joystick.h"
#include "net.h"
#include "texture.h"
#include "model.h"
#include "config_xml.h"
#include "stars.h"
#include "thread.h"
#include "scene.h"
#include "scene_xml.h"
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

#define _QUOTEME(x) #x
#define QUOTEME(x) _QUOTEME(x)

#ifdef AMETHYST_VERSTR
 #ifdef AMETHYST_WC_REVISION
#define AMETHYST_LONG_NAME  Amethyst Space Physics Simulator - AMETHYST_VERSTR (SVN: AMETHYST_WC_REVISION)
  #else
  #define AMETHYST_LONG_NAME  Amethyst Space Physics Simulator - AMETHYST_VERSTR
 #endif
 #else
 #define AMETHYST_LONG_NAME  Amethyst Space Physics Simulator
#endif

#define AMETHSYT_SHORT_NAME Amethyst-GL

namespace amethyst
{
namespace client
{


amethyst_gl::amethyst_gl(const std::string &path_root)
    : manifest_(path_root),
      connection(manifest_),
      ui("/spacefri.ttf") //FIXME make not static
{

    //connection.start("127.0.0.1", "2525", "beau", "test");
    //net_thread = new boost::thread(boost::bind(&amethyst_gl::start_net, this));
}


void amethyst_gl::main_loop()
{
    Universe &universe = Global.universe;

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
    render();
    }
}


void amethyst_gl::render()
{
    // Clear the window with current clearing color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    scene_render();

    // Display HUD
    hud_render();

    ui.render();

    // Do the buffer Swap
    SDL_GL_SwapBuffers();
}


void amethyst_gl::start_net()
{
    std::cout << "Net: Handing over control to Netwrok Thread...\n";
    std::cout << "Thread: Starting Network Thread..." << std::endl;
    connection.run();
    std::cout << "Thread: Network Thread Terminating, Nothing to do..." << std::endl;
}


} // namespace client
} // namespace amethyst


// bring some standard stuff into our namespace
using std::cout;
using std::endl;
using std::string;

static void sdl_setup(void);
static void sdl_setup(void);


static void sdl_setup()
{
    const SDL_VideoInfo* video;

    cout << "Initializing SDL...";
    if ( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
        fprintf(stderr,
                "Couldn't initialize SDL: %s\n", SDL_GetError());
        exit(1);
    } else cout << "Success!" << endl;

    atexit(SDL_Quit);


    SDL_WM_SetCaption(QUOTEME(AMETHYST_LONG_NAME),QUOTEME(AMETHSYT_SHORT_NAME));

    cout << "Initializing Video...";
    video = SDL_GetVideoInfo( );

    if( !video ) {
        fprintf(stderr,
                "Couldn't get video information: %s\n", SDL_GetError());
        exit(1);
    } else cout << "Success!" << endl;

    cout << "Initializing OpenGL (Part 1)...";

    /* Set the minimum requirements for the OpenGL window */
    SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_BUFFER_SIZE, 24 );
    SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 24 );
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

    if( SDL_SetVideoMode( WIDTH, HEIGHT, video->vfmt->BitsPerPixel, SDL_OPENGL |  SDL_RESIZABLE) == 0 )
    {
        fprintf(stderr,
                "Couldn't set video mode: %s\n", SDL_GetError());
        exit(1);
    } else cout << "Sucess!" << endl;

    int value;

    SDL_GL_GetAttribute( SDL_GL_RED_SIZE, &value);
    cout << "Video mode: R" << value << ",";
    SDL_GL_GetAttribute( SDL_GL_GREEN_SIZE, &value);
    cout << "G" << value << ",";
    SDL_GL_GetAttribute( SDL_GL_BLUE_SIZE, &value);
    cout << "B" << value << ",";
    SDL_GL_GetAttribute( SDL_GL_ALPHA_SIZE, &value);
    cout << "A" << value << "/";
    SDL_GL_GetAttribute( SDL_GL_DEPTH_SIZE, &value);
    cout << value << endl;


}





int main(int argc, char* argv[])
{
    using namespace amethyst::client;

    std::cout << QUOTEME(AMETHSYT_SHORT_NAME) << " Starting..." << std::endl;
    std::cout << " * " << argc-1 << " arguments found on command line." << std::endl;

    Global.dir_home     = getenv (PROFILE_DIR_ENV);
    Global.dir_amethyst = Global.dir_home + "/" + PROJECT_DIR;
    Global.dir_scene    = Global.dir_amethyst + "/scene/";
    Global.dir_textures = Global.dir_amethyst + "/textures/";
    Global.dir_models   = Global.dir_amethyst + "/models/";
    Global.dir_shaders  = Global.dir_amethyst + "/shaders/";
    Global.dir_fonts    = Global.dir_amethyst + "/fonts";

    string config_file  = Global.dir_amethyst + "/config.xml";
    string stars_file   = Global.dir_amethyst + "/stars.csv";

    // Check for existance of config.xml else fail siliently
    if(access(config_file.c_str(), F_OK) == 0) {

        // Check for read permission else fail kicking and screaming
        if(access(config_file.c_str(), R_OK) == 0) {
            parse_xml_config (config_file.c_str());
            std::cout << " * Reading config file at "
                      << config_file << "..." << std::endl;
        } else {
            std::cout << " ! Unable to read config file at "
                      << config_file << ", permission?" << std::endl;
        }
    }


    sdl_setup();

    opengl_setup();

    joystick_setup();

    amethyst_gl client(Global.dir_amethyst);

    // FIXME XXX network_setup();

    hud_setup();


    stars_load(stars_file);
    models_load();

    try // Experimenting with exceptions
    {
        scene_load(Global.scene);
    }
    catch(std::runtime_error &e)
    {
        std::cout << "Non-Fatal Exception: " << e.what() << std::endl;
        throw e;
    }

    // Load shaders if supported
    if (glShaderObjectsSupported)
        load_shader(Global.vshader, Global.fshader);


    client.main_loop();

    models_free();
    stars_free();

    thread_stop_all();

    SDL_Quit ();
    return EXIT_SUCCESS;
}


