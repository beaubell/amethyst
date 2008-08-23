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

GLboolean glVersion14               = GL_FALSE;
GLboolean glVersion15               = GL_FALSE;
GLboolean glWindowPosSupported      = GL_FALSE;
GLboolean glWindowPosEnabled        = GL_FALSE;
GLboolean glOcclusionQuerySupported = GL_FALSE;
GLboolean glOcclusionQueryEnabled   = GL_FALSE;

unsigned int screen_x = 1024;
unsigned int screen_y = 640;

static void opengl_ext_window_pos(void);
static void opengl_ext_occlusion_query(void);

void opengl_setup(void)
{
    std::cout << "Initializing OpenGL (Part 2)...\n";

    // Get Basic Info About the OpenGL Context
    glVersion    = glGetString(GL_VERSION);
    glExtensions = glGetString(GL_EXTENSIONS);
    printf("GL_RENDERER   = %s\n", (char *) glGetString(GL_RENDERER));
    printf("GL_VERSION    = %s\n", glVersion);
    printf("GL_VENDOR     = %s\n", (char *) glGetString(GL_VENDOR));
    printf("GL_EXTENSIONS = %s\n", glExtensions);

    // Check for OpenGL version 1.4
    if ((glVersion[0] == '1') && (glVersion[1] == '.') &&
         (glVersion[2] >= '4') && (glVersion[2] <= '9'))
    {
        glVersion14 = GL_TRUE;
    }

    // Check for OpenGL version 1.5
    if ((glVersion[0] == '1') && (glVersion[1] == '.') &&
        (glVersion[2] >= '5') && (glVersion[2] <= '9'))
    {
        glVersion15 = GL_TRUE;
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
    opengl_ext_window_pos();
    opengl_ext_occlusion_query();

    // Set aspect to default;
    opengl_change_aspect(WIDTH, HEIGHT);

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

    screen_x = w, screen_y = h;

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

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


void opengl_ext_window_pos(void)
{
    printf("Checking for GLEXT:window_pos...");

    if(!glVersion14 && !strstr((const char*)glExtensions, "GL_ARB_window_pos"))
        puts("Not Found.  :-(");

    if (glVersion14)
    {
        glWindowPos2d  = (PFNGLWINDOWPOS2DPROC)SDL_GL_GetProcAddress("glWindowPos2d");
        glWindowPos2dv = (PFNGLWINDOWPOS2DVPROC)SDL_GL_GetProcAddress("glWindowPos2dv");
        glWindowPos2f  = (PFNGLWINDOWPOS2FPROC)SDL_GL_GetProcAddress("glWindowPos2f");
        glWindowPos2fv = (PFNGLWINDOWPOS2FVPROC)SDL_GL_GetProcAddress("glWindowPos2fv");
        glWindowPos2i  = (PFNGLWINDOWPOS2IPROC)SDL_GL_GetProcAddress("glWindowPos2i");
        glWindowPos2iv = (PFNGLWINDOWPOS2IVPROC)SDL_GL_GetProcAddress("glWindowPos2iv");
        glWindowPos2s  = (PFNGLWINDOWPOS2SPROC)SDL_GL_GetProcAddress("glWindowPos2s");
        glWindowPos2sv = (PFNGLWINDOWPOS2SVPROC)SDL_GL_GetProcAddress("glWindowPos2sv");
        glWindowPos3d  = (PFNGLWINDOWPOS3DPROC)SDL_GL_GetProcAddress("glWindowPos3d");
        glWindowPos3dv = (PFNGLWINDOWPOS3DVPROC)SDL_GL_GetProcAddress("glWindowPos3dv");
        glWindowPos3f  = (PFNGLWINDOWPOS3FPROC)SDL_GL_GetProcAddress("glWindowPos3f");
        glWindowPos3fv = (PFNGLWINDOWPOS3FVPROC)SDL_GL_GetProcAddress("glWindowPos3fv");
        glWindowPos3i  = (PFNGLWINDOWPOS3IPROC)SDL_GL_GetProcAddress("glWindowPos3i");
        glWindowPos3iv = (PFNGLWINDOWPOS3IVPROC)SDL_GL_GetProcAddress("glWindowPos3iv");
        glWindowPos3s  = (PFNGLWINDOWPOS3SPROC)SDL_GL_GetProcAddress("glWindowPos3s");
        glWindowPos3sv = (PFNGLWINDOWPOS3SVPROC)SDL_GL_GetProcAddress("glWindowPos3sv");
    }else
    {
        glWindowPos2d  = (PFNGLWINDOWPOS2DPROC)SDL_GL_GetProcAddress("glWindowPos2dARB");
        glWindowPos2dv = (PFNGLWINDOWPOS2DVPROC)SDL_GL_GetProcAddress("glWindowPos2dvARB");
        glWindowPos2f  = (PFNGLWINDOWPOS2FPROC)SDL_GL_GetProcAddress("glWindowPos2fARB");
        glWindowPos2fv = (PFNGLWINDOWPOS2FVPROC)SDL_GL_GetProcAddress("glWindowPos2fvARB");
        glWindowPos2i  = (PFNGLWINDOWPOS2IPROC)SDL_GL_GetProcAddress("glWindowPos2iARB");
        glWindowPos2iv = (PFNGLWINDOWPOS2IVPROC)SDL_GL_GetProcAddress("glWindowPos2ivARB");
        glWindowPos2s  = (PFNGLWINDOWPOS2SPROC)SDL_GL_GetProcAddress("glWindowPos2sARB");
        glWindowPos2sv = (PFNGLWINDOWPOS2SVPROC)SDL_GL_GetProcAddress("glWindowPos2svARB");
        glWindowPos3d  = (PFNGLWINDOWPOS3DPROC)SDL_GL_GetProcAddress("glWindowPos3dARB");
        glWindowPos3dv = (PFNGLWINDOWPOS3DVPROC)SDL_GL_GetProcAddress("glWindowPos3dvARB");
        glWindowPos3f  = (PFNGLWINDOWPOS3FPROC)SDL_GL_GetProcAddress("glWindowPos3fARB");
        glWindowPos3fv = (PFNGLWINDOWPOS3FVPROC)SDL_GL_GetProcAddress("glWindowPos3fvARB");
        glWindowPos3i  = (PFNGLWINDOWPOS3IPROC)SDL_GL_GetProcAddress("glWindowPos3iARB");
        glWindowPos3iv = (PFNGLWINDOWPOS3IVPROC)SDL_GL_GetProcAddress("glWindowPos3ivARB");
        glWindowPos3s  = (PFNGLWINDOWPOS3SPROC)SDL_GL_GetProcAddress("glWindowPos3sARB");
        glWindowPos3sv = (PFNGLWINDOWPOS3SVPROC)SDL_GL_GetProcAddress("glWindowPos3svARB");
    }

    if (!glWindowPos2d || !glWindowPos2dv || !glWindowPos2f || !glWindowPos2fv ||
         !glWindowPos2i || !glWindowPos2iv || !glWindowPos2s || !glWindowPos2sv ||
         !glWindowPos3d || !glWindowPos3dv || !glWindowPos3f || !glWindowPos3fv ||
         !glWindowPos3i || !glWindowPos3iv || !glWindowPos3s || !glWindowPos3sv)
    {
        puts("Entry Point Failure.  :-(");
        return;
    }

    puts("Found.  :-)");

    glWindowPosSupported = GL_TRUE;
    glWindowPosEnabled   = GL_TRUE;
}


void opengl_ext_occlusion_query(void)
{
    printf("Checking for GLEXT:occlussion_query...");

    if(!glVersion15 && !strstr((const char*)glExtensions, "GL_ARB_occlusion_query"))
    {
        puts("Not Found.  :-(");
        return;
    }


    if (glVersion15)
    {
        glGenQueries       = (PFNGLGENQUERIESPROC)SDL_GL_GetProcAddress("glGenQueries");
        glDeleteQueries    = (PFNGLDELETEQUERIESPROC)SDL_GL_GetProcAddress("glDeleteQueries");
        glIsQuery          = (PFNGLISQUERYPROC)SDL_GL_GetProcAddress("glIsQuery");
        glBeginQuery       = (PFNGLBEGINQUERYPROC)SDL_GL_GetProcAddress("glBeginQuery");
        glEndQuery         = (PFNGLENDQUERYPROC)SDL_GL_GetProcAddress("glEndQuery");
        glGetQueryiv       = (PFNGLGETQUERYIVPROC)SDL_GL_GetProcAddress("glGetQueryiv");
        glGetQueryObjectiv = (PFNGLGETQUERYOBJECTIVPROC)SDL_GL_GetProcAddress("glGetQueryObjectiv");
        glGetQueryObjectuiv = (PFNGLGETQUERYOBJECTUIVPROC)SDL_GL_GetProcAddress("glGetQueryObjectuiv");

    } else
    {
        glGenQueries       = (PFNGLGENQUERIESPROC)SDL_GL_GetProcAddress("glGenQueriesARB");
        glDeleteQueries    = (PFNGLDELETEQUERIESPROC)SDL_GL_GetProcAddress("glDeleteQueriesARB");
        glIsQuery          = (PFNGLISQUERYPROC)SDL_GL_GetProcAddress("glIsQueryARB");
        glBeginQuery       = (PFNGLBEGINQUERYPROC)SDL_GL_GetProcAddress("glBeginQueryARB");
        glEndQuery         = (PFNGLENDQUERYPROC)SDL_GL_GetProcAddress("glEndQueryARB");
        glGetQueryiv       = (PFNGLGETQUERYIVPROC)SDL_GL_GetProcAddress("glGetQueryivARB");
        glGetQueryObjectiv = (PFNGLGETQUERYOBJECTIVPROC)SDL_GL_GetProcAddress("glGetQueryObjectivARB");
        glGetQueryObjectuiv = (PFNGLGETQUERYOBJECTUIVPROC)SDL_GL_GetProcAddress("glGetQueryObjectuivARB");
    }

    if (!glGenQueries || !glDeleteQueries || !glIsQuery || !glBeginQuery || !glEndQuery ||
        !glGetQueryiv || !glGetQueryObjectiv || !glGetQueryObjectuiv)
    {
        puts("Entry Point Failure.  :-(");
        return;
    }

    GLint  queryCounterBits = 0;
    glGetQueryiv(GL_SAMPLES_PASSED, GL_QUERY_COUNTER_BITS, &queryCounterBits);
    if (queryCounterBits == 0)
    {
        puts("Unusable.  :-(");
        return;
    }
    puts("Found.  :-)");

    glOcclusionQuerySupported = GL_TRUE;
    glOcclusionQueryEnabled   = GL_TRUE;
}
