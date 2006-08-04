/***************************************************************************
 *   Starfighter OpenGL Routines                                           *
 *  (c) 2006 Beau V.C. Bellamy (beau@stellarnetservices.net)               *
 ***************************************************************************/

#include "opengl.h"

#include <iostream>

#include "global.h"

PFNGLWINDOWPOS2DPROC    glWindowPos2d  = NULL;
PFNGLWINDOWPOS2DVPROC   glWindowPos2dv = NULL;
PFNGLWINDOWPOS2FPROC    glWindowPos2f  = NULL;
PFNGLWINDOWPOS2FVPROC   glWindowPos2fv = NULL;
PFNGLWINDOWPOS2IPROC    glWindowPos2i  = NULL;
PFNGLWINDOWPOS2IVPROC   glWindowPos2iv = NULL;
PFNGLWINDOWPOS2SPROC    glWindowPos2s  = NULL;
PFNGLWINDOWPOS2SVPROC   glWindowPos2sv = NULL;
PFNGLWINDOWPOS3DPROC    glWindowPos3d  = NULL;
PFNGLWINDOWPOS3DVPROC   glWindowPos3dv = NULL;
PFNGLWINDOWPOS3FPROC    glWindowPos3f  = NULL;
PFNGLWINDOWPOS3FVPROC   glWindowPos3fv = NULL;
PFNGLWINDOWPOS3IPROC    glWindowPos3i  = NULL;
PFNGLWINDOWPOS3IVPROC   glWindowPos3iv = NULL;
PFNGLWINDOWPOS3SPROC    glWindowPos3s  = NULL;
PFNGLWINDOWPOS3SVPROC   glWindowPos3sv = NULL;

PFNGLGENQUERIESPROC        glGenQueries        = NULL;
PFNGLDELETEQUERIESPROC     glDeleteQueries     = NULL;
PFNGLISQUERYPROC           glIsQuery           = NULL;
PFNGLBEGINQUERYPROC        glBeginQuery        = NULL;
PFNGLENDQUERYPROC          glEndQuery          = NULL;
PFNGLGETQUERYIVPROC        glGetQueryiv        = NULL;
PFNGLGETQUERYOBJECTIVPROC  glGetQueryObjectiv  = NULL;
PFNGLGETQUERYOBJECTUIVPROC glGetQueryObjectuiv = NULL;

GLubyte const *glVersion    = NULL;
GLubyte const *glExtensions = NULL;

GLboolean glVersion15               = GL_FALSE;
GLboolean glOcclusionQuerySupported = GL_FALSE;
GLboolean glOcclusionQueryEnabled   = GL_FALSE;


void setup_opengl(void)
{
    std::cout << "Initializing OpenGL (Part 2)...\n";

    // Get Basic Info About the OpenGL Context
    glVersion    = glGetString(GL_VERSION);
    glExtensions = glGetString(GL_EXTENSIONS);
    printf("GL_RENDERER   = %s\n", (char *) glGetString(GL_RENDERER));
    printf("GL_VERSION    = %s\n", glVersion);
    printf("GL_VENDOR     = %s\n", (char *) glGetString(GL_VENDOR));
    printf("GL_EXTENSIONS = %s\n", glExtensions);

    // Check for OpenGL version 1.5 through 1.9
    if ((glVersion[0] == '1') && (glVersion[1] == '.') &&
        (glVersion[2] >= '5') && (glVersion[2] <= '9'))
    {
        glVersion15 = GL_TRUE;
    }else
    {
        glVersion15 = GL_FALSE;
    }

    // Query point size capabilities
    GLint points[2], points_gran;
    glGetIntegerv(GL_SMOOTH_POINT_SIZE_RANGE, (GLint *)&points);
    glGetIntegerv(GL_SMOOTH_POINT_SIZE_GRANULARITY, &points_gran);
    printf("GL_POINT_SIZE = %d/%d  step(%d)\n", points[0], points[1], points_gran);

    // Query texture size capabilities
    GLint MaxSize;
    glGetIntegerv( GL_MAX_TEXTURE_SIZE, &MaxSize );
    printf("GL_MAX_TEXTURE_SIZE: %d\n", MaxSize);

    // Do opengl extension checks
    opengl_ext_occlusion_query();

    // Set aspect to default;
    opengl_change_aspect(WIDTH, HEIGHT);

    // Black background
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f );

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glEnable(GL_POINT_SMOOTH);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Set up lighting
    GLfloat fAmbLight[] =   { 0.05f, 0.05f, 0.05f, 1.00f }; // Darkish
    GLfloat fDiffLight[] =  { 1.0f, 1.0f, 1.0f, 1.0f };     // White
    GLfloat fSpecLight[] =  { 1.0f, 1.0f, 1.0f, 1.0f };     // White
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, fAmbLight);
    glLightfv(GL_LIGHT0, GL_AMBIENT, fAmbLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, fDiffLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, fSpecLight);

}


void opengl_change_aspect(GLsizei w, GLsizei h)
{

    GLfloat fAspect;

    // Prevent a divide by zero, when window is too short
    // (you cant make a window of zero width).
    if(h == 0)
        h = 1;

    glViewport(0, 0, w, h);

    fAspect = (GLfloat)w / (GLfloat)h;

    // Reset the coordinate system before modifying
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Set the clipping volume
    gluPerspective(Global.fov, fAspect, 1.0f, 50000000000.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


void opengl_ext_occlusion_query(void)
{
    if(!glVersion15 && !strstr((const char*)glExtensions, "GL_ARB_occlusion_query"))
       puts("Occlusion Querys Not Supported.   :-(\n");


    if (glVersion15)
    {
        glBeginQuery = (PFNGLBEGINQUERYPROC)SDL_GL_GetProcAddress("glBeginQuery");


    }

}
