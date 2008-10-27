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

#include "lib/manifest.h"
#include "net.h"
#include "ui.h"

#include <boost/thread.hpp>

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

    UI       ui;
   private:
    lib::filemanifest manifest_;
    server_connection connection;

    boost::thread *net_thread;
    void start_net();

    //Universe universe;

};

} // namespace client
} // namespace amethyst

#endif // AMETHYST_CLIENT_AMETHYSTGL_H
