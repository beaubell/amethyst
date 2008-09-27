#ifndef SF_OPENGL_H
#define SF_OPENGL_H

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
#include "GL/gl.h"
#include "GL/glext.h"

#include "SDL_opengl.h"

void opengl_setup(void);
void opengl_change_aspect(GLsizei w, GLsizei h);


// WindowPOS Function Pointers
extern PFNGLWINDOWPOS2DPROC  glWindowPos2d;
extern PFNGLWINDOWPOS2DVPROC glWindowPos2dv;
extern PFNGLWINDOWPOS2FPROC  glWindowPos2f;
extern PFNGLWINDOWPOS2FVPROC glWindowPos2fv;
extern PFNGLWINDOWPOS2IPROC  glWindowPos2i;
extern PFNGLWINDOWPOS2IVPROC glWindowPos2iv;
extern PFNGLWINDOWPOS2SPROC  glWindowPos2s;
extern PFNGLWINDOWPOS2SVPROC glWindowPos2sv;
extern PFNGLWINDOWPOS3DPROC  glWindowPos3d;
extern PFNGLWINDOWPOS3DVPROC glWindowPos3dv;
extern PFNGLWINDOWPOS3FPROC  glWindowPos3f;
extern PFNGLWINDOWPOS3FVPROC glWindowPos3fv;
extern PFNGLWINDOWPOS3IPROC  glWindowPos3i;
extern PFNGLWINDOWPOS3IVPROC glWindowPos3iv;
extern PFNGLWINDOWPOS3SPROC  glWindowPos3s;
extern PFNGLWINDOWPOS3SVPROC glWindowPos3sv;

// Occlusion Query Function Pointers
extern PFNGLGENQUERIESPROC        glGenQueries;
extern PFNGLDELETEQUERIESPROC     glDeleteQueries;
extern PFNGLISQUERYPROC           glIsQuery;
extern PFNGLBEGINQUERYPROC        glBeginQuery;
extern PFNGLENDQUERYPROC          glEndQuery;
extern PFNGLGETQUERYIVPROC        glGetQueryiv;
extern PFNGLGETQUERYOBJECTIVPROC  glGetQueryObjectiv;
extern PFNGLGETQUERYOBJECTUIVPROC glGetQueryObjectuiv;

// Framebuffer objects "GL_EXT_framebuffer_object"
extern PFNGLISRENDERBUFFEREXTPROC             glIsRenderbufferEXT;
extern PFNGLBINDRENDERBUFFEREXTPROC           glBindRenderbufferEXT;
extern PFNGLDELETERENDERBUFFERSEXTPROC        glDeleteRenderbuffersEXT;
extern PFNGLGENRENDERBUFFERSEXTPROC           glGenRenderbuffersEXT;
extern PFNGLRENDERBUFFERSTORAGEEXTPROC        glRenderbufferStorageEXT;
extern PFNGLGETRENDERBUFFERPARAMETERIVEXTPROC glGetRenderbufferParameterivEXT;
extern PFNGLISFRAMEBUFFEREXTPROC              glIsFramebufferEXT;
extern PFNGLBINDFRAMEBUFFEREXTPROC            glBindFramebufferEXT;
extern PFNGLDELETEFRAMEBUFFERSEXTPROC         glDeleteFramebuffersEXT;
extern PFNGLGENFRAMEBUFFERSEXTPROC            glGenFramebuffersEXT;
extern PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC     glCheckFramebufferStatusEXT;
extern PFNGLFRAMEBUFFERTEXTURE1DEXTPROC       glFramebufferTexture1DEXT;
extern PFNGLFRAMEBUFFERTEXTURE2DEXTPROC       glFramebufferTexture2DEXT;
extern PFNGLFRAMEBUFFERTEXTURE3DEXTPROC       glFramebufferTexture3DEXT;
extern PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC    glFramebufferRenderbufferEXT;
extern PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVEXTPROC glGetFramebufferAttachmentParameterivEXT;
extern PFNGLGENERATEMIPMAPEXTPROC             glGenerateMipmapEXT;

//GL_EXT_framebuffer_object

extern GLboolean glVersion14;
extern GLboolean glVersion15;
extern GLboolean glWindowPosSupported;
extern GLboolean glWindowPosEnabled;
extern GLboolean glOcclusionQuerySupported;
extern GLboolean glOcclusionQueryEnabled;
extern GLboolean glFramebufferObjectSupported;
extern GLboolean glFramebufferObjectEnabled;

extern unsigned int screen_x;
extern unsigned int screen_y;

#endif /* SF_OPENGL_H */
