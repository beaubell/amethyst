#ifndef SF_OPENGL_H
#define SF_OPENGL_H

/***************************************************************************
 *   Starfighter OpenGL Routines                                           *
 *  (c) 2006 Beau V.C. Bellamy (beau@stellarnetservices.net)               *
 ***************************************************************************/

#include <GL/gl.h>

void setup_opengl(void);
void opengl_change_aspect(GLsizei w, GLsizei h);

void opengl_ext_occlusion_query(void);

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

extern GLboolean glVersion15;
extern GLboolean glOcclusionQuerySupported;
extern GLboolean glOcclusionQueryEnabled;

#endif /* SF_OPENGL_H */
