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

extern void (*glBeginQuery)(GLenum target, GLuint id);

extern GLboolean glVersion15;
extern GLboolean glOcclusionQuerySupported;
extern GLboolean glOcclusionQueryEnabled;

#endif /* SF_OPENGL_H */
