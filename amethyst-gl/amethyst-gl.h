#ifndef AMETHYST_CLIENT_AMETHYSTGL_H
#define AMETHYST_CLIENT_AMETHYSTGL_H
/***********************************************************************
 Amethyst-GL
  - Main class declarations declarations

 Authors (c):
 2008-2008 Beau V.C. Bellamy (beau@stellarnetservices.net)

 $Revision$
 $LastChangedDate$
 $LastChangedBy$
 ***********************************************************************/

namespace amethyst {
namespace client {
  class Amethyst_GL;
}
}
#include "lib/manifest.h"
#include "net.h"
#include "ui.h"
#include "input.h"
#include "log.h"

#include <boost/thread.hpp>
#include <boost/signals2.hpp>

#include <CL/cl_platform.h>

namespace amethyst
{
namespace client
{

class Amethyst_GL
{
   public:
    Amethyst_GL(const std::string &path_root);

    void main_loop(void);
    void render   (void);

    UI        ui;
    Input    *const input;

    typedef boost::signals2::signal<void (const lib::Cartesian_Vector& reference)> Signal_render;
    Signal_render sig_render_scene;

   private:
    lib::FileManifest manifest_;
    Server_Connection connection;

    boost::thread *net_thread;
    void start_net();

    //Universe universe;

};

} // namespace client
} // namespace amethyst

#endif // AMETHYST_CLIENT_AMETHYSTGL_H
