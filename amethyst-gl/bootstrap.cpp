/***********************************************************************
 Amethyst-GL
  - Main client initialization

 Authors (c):
 2008-2020 Beau V.C. Bellamy (beau@stellarnetservices.net)
 ***********************************************************************/

#include "amethyst-gl.h"
#include "global.h"
#include "hud.h"
#include "stars.h"
#include "module.h"
#include "opengl.h"
#include "shaderprog.h"
#include "scene.h"
#include "lib/cl.h"
#include "lib/utility.h"
#include "lib/physics.h"
#include "lib/resource.h"
#include "science/gravpotential.h"
#include "science/uiw_simstats.h"
#include "science/uiw_view.h"

#include "yaml-cpp/yaml.h"
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

DECLARE_RESOURCE(amethyst_gl_resources_config_yml);
//DECLARE_RESOURCE(amethyst_gl_resources_config_yml);

// bring some standard stuff into our namespace
using std::cout;
using std::endl;
using std::string;

static void sdl_setup(void);

static void sdl_setup()
{
    
    using namespace amethyst;
    using namespace amethyst::client;
  
    cout << "Initializing SDL...";
    if ( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
        fprintf(stderr,
                "Couldn't initialize SDL: %s\n", SDL_GetError());
        exit(1);
    } else cout << "Success!" << endl;

    atexit(SDL_Quit);

    cout << "Initializing OpenGL (Part 1)...";

    // Request opengl 3.3 context.
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    //SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
    
    //SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    
    // Set the minimum requirements for the OpenGL window
    SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_BUFFER_SIZE, 24 );
    SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 24 );
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
    SDL_GL_SetAttribute( SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute( SDL_GL_MULTISAMPLESAMPLES, 4);
    //SDL_GL_SetAttribute( SDL_GL_STEREO, 1 );

    Global.mainwindow = SDL_CreateWindow(QUOTEME(AMETHYST_LONG_NAME), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

    if (!Global.mainwindow)
    {
        fprintf(stderr,
                "Couldn't set video mode: %s\n", SDL_GetError());
        exit(1);
    } else cout << "Sucess!" << endl;

    //checkSDLError(__LINE__);
    
     /* Create our opengl context and attach it to our window */
    Global.maincontext = SDL_GL_CreateContext(Global.mainwindow);
    //checkSDLError(__LINE__);
    
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

    string config_file  = "config.yml";
    string stars_file   = "stars.csv";

    sdl_setup();

    opengl_setup();

    //FIXME lib::cl_init();

    // Create Instance of Amethyst Physics Engine
    Amethyst_GL client(Global.dir_amethyst);
    
    Resource mainconfig_res = LOAD_RESOURCE(amethyst_gl_resources_config_yml, Global.dir_amethyst, config_file);
    
    auto config_is_ptr = mainconfig_res.get_istream();
    YAML::Node mainconfig = YAML::Load(*config_is_ptr);
    
    client.configure(mainconfig);

    // FIXME XXX network_setup();

    //hud_setup();

    //if  (module_manager.load("uiw_fps"))
    //    module_manager.start("uiw_fps", client);

    stars_load(Resource(Global.dir_amethyst, stars_file));
    
    try
    {
        Resource res(Global.dir_scene, "scn_Startup.yaml");

        auto scene_is_ptr = res.get_istream();
        YAML::Node root = YAML::Load(*scene_is_ptr);

        YAML::Node scene = root["scene"];

        // Load Scene YAML
        client.get_scene().fromYAML(scene);
        opengl_check_errors("bootstrap-asceneload");
    }
    catch(std::runtime_error &e)
    {
        std::cout << "Non-Fatal Exception: " << e.what() << std::endl;
        throw e;
    }

#if 0
    // This was for the Interplanetary Transport Network experiment.
    //lib::gen_model_solarsystem(Global.universe);

    lib::Object::sptr p1 = Global.universe.object_find("S-E L1 Probe");

    uint r_step[3] = {1,1,1};
    uint v_step[3] = {7,7,1};
    int r_offset[3] = {0,0,0};
    int v_offset[3] = {3,3,0};


    lib::gen_object_variation(Global.universe,
                         (*p1),
                         lib::Cartesian_Vector(100.0, 100.0, 100.0),
                         lib::Cartesian_Vector(0.5, 0.5, 0.5),
                         r_step, v_step,
                         r_offset, v_offset);
#endif
    opengl_check_errors("bootstrap-bopencl");
    // Send Objects to GPU
    std::cout << "Size of Universe" << Global.universe.object_count() << std::endl;
    try {
        Global.universe.cl_setup();
        Global.universe.cl_copytogpu();
    } catch (std::exception& ex)
    {
        Global.log.add(std::string("Exception thrown during OpenCL Setup: ")+ ex.what());
    }

    //GravPotential test(client);
    opengl_check_errors("bootstrap-bsimstats");
    
    // Create and add sim stats window
    UI_Window_ptr window = UI_Window_ptr(new UIW_SimStats(client, client.ui));
    client.ui.add(window);
    UI_Window_ptr window_view = UI_Window_ptr(new UIW_View(client, client.ui));
    client.ui.add(window_view);
    

    opengl_check_errors("bootstrap-bmain");
    
    client.main_loop();

    models_free();
    textures_free();
    stars_free();

    //thread_stop_all();

    //module_manager.stop_all();

    module_manager.unload_all();
    
    SDL_GL_DeleteContext(Global.maincontext);
    SDL_DestroyWindow(Global.mainwindow);
    SDL_QuitSubSystem(SDL_INIT_VIDEO);

    SDL_Quit ();
    return EXIT_SUCCESS;
}
