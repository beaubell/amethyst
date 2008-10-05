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

// Shader objects "GL_ARB_shader_objects"
extern PFNGLDELETEOBJECTARBPROC               glDeleteObjectARB;
extern PFNGLGETHANDLEARBPROC                  glGetHandleARB;
extern PFNGLDETACHOBJECTARBPROC               glDetachObjectARB;
extern PFNGLCREATESHADEROBJECTARBPROC         glCreateShaderObjectARB;
extern PFNGLSHADERSOURCEARBPROC               glShaderSourceARB;
extern PFNGLCOMPILESHADERARBPROC              glCompileShaderARB;
extern PFNGLCREATEPROGRAMOBJECTARBPROC        glCreateProgramObjectARB;
extern PFNGLATTACHOBJECTARBPROC               glAttachObjectARB;
extern PFNGLLINKPROGRAMARBPROC                glLinkProgramARB;
extern PFNGLUSEPROGRAMOBJECTARBPROC           glUseProgramObjectARB;
extern PFNGLVALIDATEPROGRAMARBPROC            glValidateProgramARB;
extern PFNGLUNIFORM1FARBPROC                  glUniform1fARB;
extern PFNGLUNIFORM2FARBPROC                  glUniform2fARB;
extern PFNGLUNIFORM3FARBPROC                  glUniform3fARB;
extern PFNGLUNIFORM4FARBPROC                  glUniform4fARB;
extern PFNGLUNIFORM1IARBPROC                  glUniform1iARB;
extern PFNGLUNIFORM2IARBPROC                  glUniform2iARB;
extern PFNGLUNIFORM3IARBPROC                  glUniform3iARB;
extern PFNGLUNIFORM4IARBPROC                  glUniform4iARB;
extern PFNGLUNIFORM1FVARBPROC                 glUniform1fvARB;
extern PFNGLUNIFORM2FVARBPROC                 glUniform2fvARB;
extern PFNGLUNIFORM3FVARBPROC                 glUniform3fvARB;
extern PFNGLUNIFORM4FVARBPROC                 glUniform4fvARB;
extern PFNGLUNIFORM1IVARBPROC                 glUniform1ivARB;
extern PFNGLUNIFORM2IVARBPROC                 glUniform2ivARB;
extern PFNGLUNIFORM3IVARBPROC                 glUniform3ivARB;
extern PFNGLUNIFORM4IVARBPROC                 glUniform4ivARB;
extern PFNGLUNIFORMMATRIX2FVARBPROC           glUniformMatrix2fvARB;
extern PFNGLUNIFORMMATRIX3FVARBPROC           glUniformMatrix3fvARB;
extern PFNGLUNIFORMMATRIX4FVARBPROC           glUniformMatrix4fvARB;
extern PFNGLGETOBJECTPARAMETERFVARBPROC       glGetObjectParameterfvARB;
extern PFNGLGETOBJECTPARAMETERIVARBPROC       glGetObjectParameterivARB;
extern PFNGLGETINFOLOGARBPROC                 glGetInfoLogARB;
extern PFNGLGETATTACHEDOBJECTSARBPROC         glGetAttachedObjectsARB;
extern PFNGLGETUNIFORMLOCATIONARBPROC         glGetUniformLocationARB;
extern PFNGLGETACTIVEUNIFORMARBPROC           glGetActiveUniformARB;
extern PFNGLGETUNIFORMFVARBPROC               glGetUniformfvARB;
extern PFNGLGETUNIFORMIVARBPROC               glGetUniformivARB;
extern PFNGLGETSHADERSOURCEARBPROC            glGetShaderSourceARB;



// Shaders!!
extern PFNGLBINDATTRIBLOCATIONARBPROC         glBindAttribLocationARB;
extern PFNGLGETACTIVEATTRIBARBPROC            glGetActiveAttribARB;
extern PFNGLGETATTRIBLOCATIONARBPROC          glGetAttribLocationARB;


extern GLboolean glVersion14;
extern GLboolean glVersion15;
extern GLboolean glWindowPosSupported;
extern GLboolean glWindowPosEnabled;
extern GLboolean glOcclusionQuerySupported;
extern GLboolean glOcclusionQueryEnabled;
extern GLboolean glFramebufferObjectSupported;
extern GLboolean glFramebufferObjectEnabled;
extern GLboolean glShaderObjectsSupported;
extern GLboolean glShaderObjectsEnabled;

extern unsigned int screen_x;
extern unsigned int screen_y;

#endif /* SF_OPENGL_H */
