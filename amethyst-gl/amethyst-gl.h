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

namespace amethyst
{
namespace client
{

class amethyst_gl
{
   public:
    amethyst_gl(const std::string &path_root);

   private:
    lib::filemanifest manifest_;
    //server_connection connection;

};

} // namespace client
} // namespace amethyst

#endif // AMETHYST_CLIENT_AMETHYSTGL_H
