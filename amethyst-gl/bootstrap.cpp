/***********************************************************************
 Amethyst-GL
  - ??? FIXME ??? implementations

 Authors (c):
 2008-2008 Beau V.C. Bellamy (beau@stellarnetservices.net)

 $Revision$
 $LastChangedDate$
 $LastChangedBy$
 ***********************************************************************/

#include "amethyst-gl.h"
#include "global.h"
#include "hud.h"
#include "stars.h"
#include "module.h"
#include "opengl.h"
#include "opengl_shader.h"
#include "scene.h"
#include "scene_xml.h"
#include "config_xml.h"
#include "cl.h"
#include "lib/utility.h"
#include "physics.h"
#include "science/gravpotential.h"

#include <iostream>

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


// bring some standard stuff into our namespace
using std::cout;
using std::endl;
using std::string;

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
    //SDL_GL_SetAttribute( SDL_GL_STEREO, 1 );

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
    using namespace amethyst;
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
    Global.dir_modules = Global.dir_amethyst + "/modules/";

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

    lib::cl_init();

    // Create Instance of Amethyst Physics Engine
    Amethyst_GL client(Global.dir_amethyst);

    // FIXME XXX network_setup();

    //hud_setup();

    //if  (module_manager.load("uiw_fps"))
    //    module_manager.start("uiw_fps", client);


    stars_load(stars_file);

    try // Experimenting with exceptions
    {
        scene_load(Global.scene);
    }
    catch(std::runtime_error &e)
    {
        std::cout << "Non-Fatal Exception: " << e.what() << std::endl;
        throw e;
    }

    lib::gen_model_solarsystem(Global.universe);

    lib::Object::ptr p1 = Global.universe.object_find("S-E L1 Probe");

    uint r_step[3] = {1,1,1};
    uint v_step[3] = {7,7,1};
    int r_offset[3] = {0,0,0};
    int v_offset[3] = {3,3,0};

#if 0
    lib::gen_object_variation(Global.universe,
                         (*p1),
                         lib::Cartesian_Vector(100.0, 100.0, 100.0),
                         lib::Cartesian_Vector(0.5, 0.5, 0.5),
                         r_step, v_step,
                         r_offset, v_offset);
#endif

    // Send Objects to GPU
    std::cout << "Size of Universe" << Global.universe.object_count() << std::endl;
    Global.universe.cl_setup();
    Global.universe.cl_copytogpu();

    //GravPotential test(client);
    
    // Load shaders if supported
    //if (glShaderObjectsSupported)
    //    load_shader(Global.vshader, Global.fshader);

    
    client.main_loop();

    models_free();
    stars_free();

    thread_stop_all();

    //module_manager.stop_all();

    module_manager.unload_all();

    SDL_Quit ();
    return EXIT_SUCCESS;
}
