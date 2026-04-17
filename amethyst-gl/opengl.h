#ifndef AMETHYST_CLIENT_OPENGL_H
#define AMETHYST_CLIENT_OPENGL_H

/***********************************************************************
 Amethyst-GL
  - Network setup and processing function prototypes

 Authors (c):
 2006-2026 Beau V.C. Bellamy (bellamy.beau@gmail.com)
 ***********************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef WIN32
#include <windows.h>
#endif

#define GL_GLEXT_PROTOTYPES true
#include "GL/glcorearb.h"

#include <string>

namespace amethyst::client {

void opengl_setup(void);
void opengl_change_aspect(GLsizei w, GLsizei h);
int  opengl_check_errors(const std::string &where);

} // namespace amethyst::client

#endif /* AMETHYST_CLIENT_OPENGL_H */
