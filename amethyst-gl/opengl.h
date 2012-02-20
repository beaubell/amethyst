#ifndef AMETHYST_CLIENT_OPENGL_H
#define AMETHYST_CLIENT_OPENGL_H

/***********************************************************************
 Amethyst-GL
  - Network setup and processing function prototypes

 Authors (c):
 2006-2008 Beau V.C. Bellamy (beau@stellarnetservices.net)

 $Revision$
 $LastChangedDate$
 $LastChangedBy$
 ***********************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef WIN32
#include <windows.h>
#endif

#define NO_SDL_GLEXT 1
#include <GL/glew.h>
#include "GL/gl.h"
#include "GL/glext.h"

#include "SDL_opengl.h"

namespace amethyst {
namespace client {

void opengl_setup(void);
void opengl_change_aspect(GLsizei w, GLsizei h);

} // namespace amethyst
} // namespace client

#endif /* AMETHYST_CLIENT_OPENGL_H */
