#pragma once
/***********************************************************************
 Amethyst-GL
  - Main class declarations declarations

 Authors (c):
 2008-2020 Beau V.C. Bellamy (bellamy.beau@gmail.com)
 ***********************************************************************/

namespace amethyst {
namespace client {
  class Amethyst_GL;
}
}

#include "opengl.h"
#include "lib/manifest.h"
#include "net.h"
#include "ui.h"
#include "input.h"
#include "log.h"
#include "framebuffer.h"
#include "renderbuffer.h"
#include "texture.h"
#include "bufferobj.h"
#include "shaderprog.h"
#include "scene.h"

#include "yaml-cpp/node/node.h"

#include <boost/thread.hpp>
#include <boost/signals2.hpp>

namespace amethyst
{
namespace client
{

class Amethyst_GL
{
   public:
    Amethyst_GL(const std::string &path_root);
    virtual ~Amethyst_GL();

    void setupFramebuffers(void);
    void main_loop(void);
    void render   (void);

    UI        ui;
    Input    *const input;

    typedef boost::signals2::signal<void (const lib::Cartesian_Vector& reference)> Signal_render;
    Signal_render sig_render_scene;

    void pause_toggle();
    void ui_toggle();
    void hud_toggle();
    void fullscn_toggle();
    void stereo_toggle();
    void simint_inc();
    void simint_dec();
    void stride_inc();
    void stride_dec();
    void state_save();
    void state_recall();
    void configure(YAML::Node& config);

    Scene& get_scene();
    const Scene& get_scene() const;

    double time_scalar;


   private:
    lib::FileManifest manifest_;
    Server_Connection connection;

    boost::thread *net_thread;
    void start_net();

    //Universe universe;
    bool paused;
    unsigned int  stride;
    double simulation_interval;

    bool show_ui;
    bool show_hud;
    bool fullscreen;
    bool stereo;

    Scene scene_;

    //Framebuffers
    FrameBuffer fbleft_;
    FrameBuffer fbright_;
    Texture2D   texleft_;
    Texture2D   texright_;
    RenderBuffer rbleft_;
    RenderBuffer rbright_;
    ShaderProgram fbshader_;

    VertexArray vao_;
    ArrayBuffer vob_;

    ShaderProgram::UniformHDL shadereye_;
};

} // namespace client
} // namespace amethyst
