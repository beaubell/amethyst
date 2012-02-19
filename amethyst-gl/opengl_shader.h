#ifndef AMETHYST_CLIENT_OPENGL_SHADER_H
#define AMETHYST_CLIENT_OPENGL_SHADER_H

/***********************************************************************
 Amethyst-GL
  - OpenGL shaders setup and processing function declarations

 Authors (c):
 2008 Beau V.C. Bellamy (beau@stellarnetservices.net)

 $Revision$
 $LastChangedDate$
 $LastChangedBy$
 ***********************************************************************/

#include <string>

namespace amethyst {
namespace client {

int load_shader(const std::string &vname, const std::string &fname);
void switch_shader(int num);

} // namespace amethyst
} // namespace client

#endif // AMETHYST_CLIENT_OPENGL_SHADER_H
