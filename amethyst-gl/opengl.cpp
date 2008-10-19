/***********************************************************************
 Amethyst-GL
  - OpenGL setup and processing function implemenations

 Authors (c):
 2006-2008 Beau V.C. Bellamy (beau@stellarnetservices.net)

 $Revision$
 $LastChangedDate$
 $LastChangedBy$
 ***********************************************************************/

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

PFNGLISRENDERBUFFEREXTPROC             glIsRenderbufferEXT             = NULL;
PFNGLBINDRENDERBUFFEREXTPROC           glBindRenderbufferEXT           = NULL;
PFNGLDELETERENDERBUFFERSEXTPROC        glDeleteRenderbuffersEXT        = NULL;
PFNGLGENRENDERBUFFERSEXTPROC           glGenRenderbuffersEXT           = NULL;
PFNGLRENDERBUFFERSTORAGEEXTPROC        glRenderbufferStorageEXT        = NULL;
PFNGLGETRENDERBUFFERPARAMETERIVEXTPROC glGetRenderbufferParameterivEXT = NULL;
PFNGLISFRAMEBUFFEREXTPROC              glIsFramebufferEXT              = NULL;
PFNGLBINDFRAMEBUFFEREXTPROC            glBindFramebufferEXT            = NULL;
PFNGLDELETEFRAMEBUFFERSEXTPROC         glDeleteFramebuffersEXT         = NULL;
PFNGLGENFRAMEBUFFERSEXTPROC            glGenFramebuffersEXT            = NULL;
PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC     glCheckFramebufferStatusEXT     = NULL;
PFNGLFRAMEBUFFERTEXTURE1DEXTPROC       glFramebufferTexture1DEXT       = NULL;
PFNGLFRAMEBUFFERTEXTURE2DEXTPROC       glFramebufferTexture2DEXT       = NULL;
PFNGLFRAMEBUFFERTEXTURE3DEXTPROC       glFramebufferTexture3DEXT       = NULL;
PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC    glFramebufferRenderbufferEXT    = NULL;
PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVEXTPROC glGetFramebufferAttachmentParameterivEXT = NULL;
PFNGLGENERATEMIPMAPEXTPROC             glGenerateMipmapEXT             = NULL;

PFNGLDELETEOBJECTARBPROC               glDeleteObjectARB         = NULL;
PFNGLGETHANDLEARBPROC                  glGetHandleARB            = NULL;
PFNGLDETACHOBJECTARBPROC               glDetachObjectARB         = NULL;
PFNGLCREATESHADEROBJECTARBPROC         glCreateShaderObjectARB   = NULL;
PFNGLSHADERSOURCEARBPROC               glShaderSourceARB         = NULL;
PFNGLCOMPILESHADERARBPROC              glCompileShaderARB        = NULL;
PFNGLCREATEPROGRAMOBJECTARBPROC        glCreateProgramObjectARB  = NULL;
PFNGLATTACHOBJECTARBPROC               glAttachObjectARB         = NULL;
PFNGLLINKPROGRAMARBPROC                glLinkProgramARB          = NULL;
PFNGLUSEPROGRAMOBJECTARBPROC           glUseProgramObjectARB     = NULL;
PFNGLVALIDATEPROGRAMARBPROC            glValidateProgramARB      = NULL;
PFNGLUNIFORM1FARBPROC                  glUniform1fARB            = NULL;
PFNGLUNIFORM2FARBPROC                  glUniform2fARB            = NULL;
PFNGLUNIFORM3FARBPROC                  glUniform3fARB            = NULL;
PFNGLUNIFORM4FARBPROC                  glUniform4fARB            = NULL;
PFNGLUNIFORM1IARBPROC                  glUniform1iARB            = NULL;
PFNGLUNIFORM2IARBPROC                  glUniform2iARB            = NULL;
PFNGLUNIFORM3IARBPROC                  glUniform3iARB            = NULL;
PFNGLUNIFORM4IARBPROC                  glUniform4iARB            = NULL;
PFNGLUNIFORM1FVARBPROC                 glUniform1fvARB           = NULL;
PFNGLUNIFORM2FVARBPROC                 glUniform2fvARB           = NULL;
PFNGLUNIFORM3FVARBPROC                 glUniform3fvARB           = NULL;
PFNGLUNIFORM4FVARBPROC                 glUniform4fvARB           = NULL;
PFNGLUNIFORM1IVARBPROC                 glUniform1ivARB           = NULL;
PFNGLUNIFORM2IVARBPROC                 glUniform2ivARB           = NULL;
PFNGLUNIFORM3IVARBPROC                 glUniform3ivARB           = NULL;
PFNGLUNIFORM4IVARBPROC                 glUniform4ivARB           = NULL;
PFNGLUNIFORMMATRIX2FVARBPROC           glUniformMatrix2fvARB     = NULL;
PFNGLUNIFORMMATRIX3FVARBPROC           glUniformMatrix3fvARB     = NULL;
PFNGLUNIFORMMATRIX4FVARBPROC           glUniformMatrix4fvARB     = NULL;
PFNGLGETOBJECTPARAMETERFVARBPROC       glGetObjectParameterfvARB = NULL;
PFNGLGETOBJECTPARAMETERIVARBPROC       glGetObjectParameterivARB = NULL;
PFNGLGETINFOLOGARBPROC                 glGetInfoLogARB           = NULL;
PFNGLGETATTACHEDOBJECTSARBPROC         glGetAttachedObjectsARB   = NULL;
PFNGLGETUNIFORMLOCATIONARBPROC         glGetUniformLocationARB   = NULL;
PFNGLGETACTIVEUNIFORMARBPROC           glGetActiveUniformARB     = NULL;
PFNGLGETUNIFORMFVARBPROC               glGetUniformfvARB         = NULL;
PFNGLGETUNIFORMIVARBPROC               glGetUniformivARB         = NULL;
PFNGLGETSHADERSOURCEARBPROC            glGetShaderSourceARB      = NULL;


GLubyte const *glRenderer   = NULL;
GLubyte const *glVendor     = NULL;
GLubyte const *glVersion    = NULL;
GLubyte const *glExtensions = NULL;


GLboolean glVersion14               = GL_FALSE;
GLboolean glVersion15               = GL_FALSE;
GLboolean glWindowPosSupported      = GL_FALSE;
GLboolean glWindowPosEnabled        = GL_FALSE;
GLboolean glOcclusionQuerySupported = GL_FALSE;
GLboolean glOcclusionQueryEnabled   = GL_FALSE;
GLboolean glFramebufferObjectSupported = GL_FALSE;
GLboolean glFramebufferObjectEnabled   = GL_FALSE;
GLboolean glShaderObjectsSupported  = GL_FALSE;
GLboolean glShaderObjectsEnabled    = GL_FALSE;

static void opengl_ext_window_pos(void);
static void opengl_ext_occlusion_query(void);
static void opengl_ext_framebuffer_object(void);
static void opengl_arb_shader_objects(void);

void opengl_setup(void)
{
    std::cout << "Initializing OpenGL (Part 2)..." << std::endl;

    // Get Basic Info About the OpenGL Context
    glRenderer   = glGetString(GL_RENDERER);
    glVendor     = glGetString(GL_VENDOR);
    glVersion    = glGetString(GL_VERSION);
    glExtensions = glGetString(GL_EXTENSIONS);
    std::cout << "GL_RENDERER   = " << glRenderer << std::endl;
    std::cout << "GL_VERSION    = " << glVersion << std::endl;
    std::cout << "GL_VENDOR     = " << glVendor << std::endl;
    std::cout << "GL_EXTENSIONS = " << glExtensions << std::endl;

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
    glGetIntegerv(GL_SMOOTH_POINT_SIZE_RANGE, reinterpret_cast<GLint *>(&points));
    glGetIntegerv(GL_SMOOTH_POINT_SIZE_GRANULARITY, &points_gran);
    std::cout << "GL_POINT_SIZE = " << points[0] << "/" << points[1]
              << "  step(" << points_gran << ")" << std::endl;

    // Query texture size capabilities
    GLint MaxSize;
    glGetIntegerv( GL_MAX_TEXTURE_SIZE, &MaxSize );
    std::cout << "GL_MAX_TEXTURE_SIZE: " << MaxSize << std::endl;

    // Do opengl extension checks
    opengl_ext_window_pos();
    opengl_ext_occlusion_query();
    opengl_ext_framebuffer_object();
    opengl_arb_shader_objects();

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

    fAspect = static_cast<GLfloat>(w) / static_cast<GLfloat>(h);

    // Reset the coordinate system before modifying
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Set the clipping volume
    gluPerspective(Global.fov, fAspect, 5.0f, 5e9f);

    Global.screen_x = w;
    Global.screen_y = h;

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
    glFrontFace(GL_CCW);

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

    glDepthFunc(GL_LEQUAL);
    //glDepthRange(5.0,1e10);
    glPolygonOffset(1.0,1.0);
}


static void opengl_ext_window_pos(void)
{
    std::cout << "Checking for GLEXT:window_pos...";

    if(!glVersion14 && !strstr(reinterpret_cast<const char*>(glExtensions), "GL_ARB_window_pos"))
    {
        std::cout << "Not Found.  :-(" << std::endl;
        return;
    }

    if (glVersion14)
    {
        glWindowPos2d  = reinterpret_cast<PFNGLWINDOWPOS2DPROC>
                         (SDL_GL_GetProcAddress("glWindowPos2d"));
        glWindowPos2dv = reinterpret_cast<PFNGLWINDOWPOS2DVPROC>
                         (SDL_GL_GetProcAddress("glWindowPos2dv"));
        glWindowPos2f  = reinterpret_cast<PFNGLWINDOWPOS2FPROC>
                         (SDL_GL_GetProcAddress("glWindowPos2f"));
        glWindowPos2fv = reinterpret_cast<PFNGLWINDOWPOS2FVPROC>
                         (SDL_GL_GetProcAddress("glWindowPos2fv"));
        glWindowPos2i  = reinterpret_cast<PFNGLWINDOWPOS2IPROC>
                         (SDL_GL_GetProcAddress("glWindowPos2i"));
        glWindowPos2iv = reinterpret_cast<PFNGLWINDOWPOS2IVPROC>
                         (SDL_GL_GetProcAddress("glWindowPos2iv"));
        glWindowPos2s  = reinterpret_cast<PFNGLWINDOWPOS2SPROC>
                         (SDL_GL_GetProcAddress("glWindowPos2s"));
        glWindowPos2sv = reinterpret_cast<PFNGLWINDOWPOS2SVPROC>
                         (SDL_GL_GetProcAddress("glWindowPos2sv"));
        glWindowPos3d  = reinterpret_cast<PFNGLWINDOWPOS3DPROC>
                         (SDL_GL_GetProcAddress("glWindowPos3d"));
        glWindowPos3dv = reinterpret_cast<PFNGLWINDOWPOS3DVPROC>
                         (SDL_GL_GetProcAddress("glWindowPos3dv"));
        glWindowPos3f  = reinterpret_cast<PFNGLWINDOWPOS3FPROC>
                         (SDL_GL_GetProcAddress("glWindowPos3f"));
        glWindowPos3fv = reinterpret_cast<PFNGLWINDOWPOS3FVPROC>
                         (SDL_GL_GetProcAddress("glWindowPos3fv"));
        glWindowPos3i  = reinterpret_cast<PFNGLWINDOWPOS3IPROC>
                         (SDL_GL_GetProcAddress("glWindowPos3i"));
        glWindowPos3iv = reinterpret_cast<PFNGLWINDOWPOS3IVPROC>
                         (SDL_GL_GetProcAddress("glWindowPos3iv"));
        glWindowPos3s  = reinterpret_cast<PFNGLWINDOWPOS3SPROC>
                         (SDL_GL_GetProcAddress("glWindowPos3s"));
        glWindowPos3sv = reinterpret_cast<PFNGLWINDOWPOS3SVPROC>
                         (SDL_GL_GetProcAddress("glWindowPos3sv"));
    }else
    {
        glWindowPos2d  = reinterpret_cast<PFNGLWINDOWPOS2DPROC>
                         (SDL_GL_GetProcAddress("glWindowPos2dARB"));
        glWindowPos2dv = reinterpret_cast<PFNGLWINDOWPOS2DVPROC>
                         (SDL_GL_GetProcAddress("glWindowPos2dvARB"));
        glWindowPos2f  = reinterpret_cast<PFNGLWINDOWPOS2FPROC>
                         (SDL_GL_GetProcAddress("glWindowPos2fARB"));
        glWindowPos2fv = reinterpret_cast<PFNGLWINDOWPOS2FVPROC>
                         (SDL_GL_GetProcAddress("glWindowPos2fvARB"));
        glWindowPos2i  = reinterpret_cast<PFNGLWINDOWPOS2IPROC>
                         (SDL_GL_GetProcAddress("glWindowPos2iARB"));
        glWindowPos2iv = reinterpret_cast<PFNGLWINDOWPOS2IVPROC>
                         (SDL_GL_GetProcAddress("glWindowPos2ivARB"));
        glWindowPos2s  = reinterpret_cast<PFNGLWINDOWPOS2SPROC>
                         (SDL_GL_GetProcAddress("glWindowPos2sARB"));
        glWindowPos2sv = reinterpret_cast<PFNGLWINDOWPOS2SVPROC>
                         (SDL_GL_GetProcAddress("glWindowPos2svARB"));
        glWindowPos3d  = reinterpret_cast<PFNGLWINDOWPOS3DPROC>
                         (SDL_GL_GetProcAddress("glWindowPos3dARB"));
        glWindowPos3dv = reinterpret_cast<PFNGLWINDOWPOS3DVPROC>
                         (SDL_GL_GetProcAddress("glWindowPos3dvARB"));
        glWindowPos3f  = reinterpret_cast<PFNGLWINDOWPOS3FPROC>
                         (SDL_GL_GetProcAddress("glWindowPos3fARB"));
        glWindowPos3fv = reinterpret_cast<PFNGLWINDOWPOS3FVPROC>
                         (SDL_GL_GetProcAddress("glWindowPos3fvARB"));
        glWindowPos3i  = reinterpret_cast<PFNGLWINDOWPOS3IPROC>
                         (SDL_GL_GetProcAddress("glWindowPos3iARB"));
        glWindowPos3iv = reinterpret_cast<PFNGLWINDOWPOS3IVPROC>
                         (SDL_GL_GetProcAddress("glWindowPos3ivARB"));
        glWindowPos3s  = reinterpret_cast<PFNGLWINDOWPOS3SPROC>
                         (SDL_GL_GetProcAddress("glWindowPos3sARB"));
        glWindowPos3sv = reinterpret_cast<PFNGLWINDOWPOS3SVPROC>
                         (SDL_GL_GetProcAddress("glWindowPos3svARB"));
    }

    if (!glWindowPos2d || !glWindowPos2dv || !glWindowPos2f || !glWindowPos2fv ||
         !glWindowPos2i || !glWindowPos2iv || !glWindowPos2s || !glWindowPos2sv ||
         !glWindowPos3d || !glWindowPos3dv || !glWindowPos3f || !glWindowPos3fv ||
         !glWindowPos3i || !glWindowPos3iv || !glWindowPos3s || !glWindowPos3sv)
    {
        std::cout << "Entry Point Failure.  :-(" << std::endl;
        return;
    }

    std::cout << "Found.  :-)" << std::endl;

    glWindowPosSupported = GL_TRUE;
    glWindowPosEnabled   = GL_TRUE;
}


static void opengl_ext_occlusion_query(void)
{
    std::cout << "Checking for GLEXT:occlussion_query...";

    if(!glVersion15 && !strstr(reinterpret_cast<const char*>(glExtensions), "GL_ARB_occlusion_query"))
    {
        std::cout << "Not Found.  :-(" << std::endl;
        return;
    }


    if (glVersion15)
    {
        glGenQueries       = reinterpret_cast<PFNGLGENQUERIESPROC>
                             (SDL_GL_GetProcAddress("glGenQueries"));
        glDeleteQueries    = reinterpret_cast<PFNGLDELETEQUERIESPROC>
                             (SDL_GL_GetProcAddress("glDeleteQueries"));
        glIsQuery          = reinterpret_cast<PFNGLISQUERYPROC>
                             (SDL_GL_GetProcAddress("glIsQuery"));
        glBeginQuery       = reinterpret_cast<PFNGLBEGINQUERYPROC>
                             (SDL_GL_GetProcAddress("glBeginQuery"));
        glEndQuery         = reinterpret_cast<PFNGLENDQUERYPROC>
                             (SDL_GL_GetProcAddress("glEndQuery"));
        glGetQueryiv       = reinterpret_cast<PFNGLGETQUERYIVPROC>
                             (SDL_GL_GetProcAddress("glGetQueryiv"));
        glGetQueryObjectiv = reinterpret_cast<PFNGLGETQUERYOBJECTIVPROC>
                             (SDL_GL_GetProcAddress("glGetQueryObjectiv"));
        glGetQueryObjectuiv = reinterpret_cast<PFNGLGETQUERYOBJECTUIVPROC>
                              (SDL_GL_GetProcAddress("glGetQueryObjectuiv"));

    } else
    {
        glGenQueries       = reinterpret_cast<PFNGLGENQUERIESPROC>
                             (SDL_GL_GetProcAddress("glGenQueriesARB"));
        glDeleteQueries    = reinterpret_cast<PFNGLDELETEQUERIESPROC>
                             (SDL_GL_GetProcAddress("glDeleteQueriesARB"));
        glIsQuery          = reinterpret_cast<PFNGLISQUERYPROC>
                             (SDL_GL_GetProcAddress("glIsQueryARB"));
        glBeginQuery       = reinterpret_cast<PFNGLBEGINQUERYPROC>
                             (SDL_GL_GetProcAddress("glBeginQueryARB"));
        glEndQuery         = reinterpret_cast<PFNGLENDQUERYPROC>
                             (SDL_GL_GetProcAddress("glEndQueryARB"));
        glGetQueryiv       = reinterpret_cast<PFNGLGETQUERYIVPROC>
                             (SDL_GL_GetProcAddress("glGetQueryivARB"));
        glGetQueryObjectiv = reinterpret_cast<PFNGLGETQUERYOBJECTIVPROC>
                             (SDL_GL_GetProcAddress("glGetQueryObjectivARB"));
        glGetQueryObjectuiv = reinterpret_cast<PFNGLGETQUERYOBJECTUIVPROC>
                              (SDL_GL_GetProcAddress("glGetQueryObjectuivARB"));
    }

    if (!glGenQueries || !glDeleteQueries || !glIsQuery || !glBeginQuery || !glEndQuery ||
        !glGetQueryiv || !glGetQueryObjectiv || !glGetQueryObjectuiv)
    {
        std::cout << "Entry Point Failure.  :-(" << std::endl;
        return;
    }

    GLint  queryCounterBits = 0;
    glGetQueryiv(GL_SAMPLES_PASSED, GL_QUERY_COUNTER_BITS, &queryCounterBits);
    if (queryCounterBits == 0)
    {
        std::cout << "Unusable.  :-(" << std::endl;
        return;
    }
    std::cout << "Found.  :-)" << std::endl;

    glOcclusionQuerySupported = GL_TRUE;
    glOcclusionQueryEnabled   = GL_TRUE;
}


static void opengl_ext_framebuffer_object(void)
{
    std::cout << "Checking for GLEXT:framebuffer_ojbect...";

    if(!glVersion15 && !strstr(reinterpret_cast<const char*>(glExtensions), "GL_EXT_framebuffer_object"))
    {
        std::cout << "Not Found.  :-(" << std::endl;
        return;
    }

    glIsRenderbufferEXT              = reinterpret_cast<PFNGLISRENDERBUFFEREXTPROC>
                                       (SDL_GL_GetProcAddress("glIsRenderbufferEXT"));
    glBindRenderbufferEXT            = reinterpret_cast<PFNGLBINDRENDERBUFFEREXTPROC>
                                       (SDL_GL_GetProcAddress("glBindRenderbufferEXT"));
    glDeleteRenderbuffersEXT         = reinterpret_cast<PFNGLDELETERENDERBUFFERSEXTPROC>
                                       (SDL_GL_GetProcAddress("glDeleteRenderbuffersEXT"));
    glGenRenderbuffersEXT            = reinterpret_cast<PFNGLGENRENDERBUFFERSEXTPROC>
                                       (SDL_GL_GetProcAddress("glGenRenderbuffersEXT"));
    glRenderbufferStorageEXT         = reinterpret_cast<PFNGLRENDERBUFFERSTORAGEEXTPROC>
                                       (SDL_GL_GetProcAddress("glRenderbufferStorageEXT"));
    glGetRenderbufferParameterivEXT  = reinterpret_cast<PFNGLGETRENDERBUFFERPARAMETERIVEXTPROC>
                                       (SDL_GL_GetProcAddress("glGetRenderbufferParameterivEXT"));
    glIsFramebufferEXT               = reinterpret_cast<PFNGLISFRAMEBUFFEREXTPROC>
                                       (SDL_GL_GetProcAddress("glIsFramebufferEXT"));
    glBindFramebufferEXT             = reinterpret_cast<PFNGLBINDFRAMEBUFFEREXTPROC>
                                       (SDL_GL_GetProcAddress("glBindFramebufferEXT"));
    glDeleteFramebuffersEXT          = reinterpret_cast<PFNGLDELETEFRAMEBUFFERSEXTPROC>
                                       (SDL_GL_GetProcAddress("glDeleteFramebuffersEXT"));
    glGenFramebuffersEXT             = reinterpret_cast<PFNGLGENFRAMEBUFFERSEXTPROC>
                                       (SDL_GL_GetProcAddress("glGenFramebuffersEXT"));
    glCheckFramebufferStatusEXT      = reinterpret_cast<PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC>
                                       (SDL_GL_GetProcAddress("glCheckFramebufferStatusEXT"));
    glFramebufferTexture1DEXT        = reinterpret_cast<PFNGLFRAMEBUFFERTEXTURE1DEXTPROC>
                                       (SDL_GL_GetProcAddress("glFramebufferTexture1DEXT"));
    glFramebufferTexture2DEXT        = reinterpret_cast<PFNGLFRAMEBUFFERTEXTURE2DEXTPROC>
                                       (SDL_GL_GetProcAddress("glFramebufferTexture2DEXT"));
    glFramebufferTexture3DEXT        = reinterpret_cast<PFNGLFRAMEBUFFERTEXTURE3DEXTPROC>
                                       (SDL_GL_GetProcAddress("glFramebufferTexture3DEXT"));
    glFramebufferRenderbufferEXT     = reinterpret_cast<PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC>
                                       (SDL_GL_GetProcAddress("glFramebufferRenderbufferEXT"));
    glGetFramebufferAttachmentParameterivEXT = reinterpret_cast<PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVEXTPROC>
                                       (SDL_GL_GetProcAddress("glGetFramebufferAttachmentParameterivEXT"));
    glGenerateMipmapEXT              = reinterpret_cast<PFNGLGENERATEMIPMAPEXTPROC>
                                       (SDL_GL_GetProcAddress("glGenerateMipmapEXT"));

    if (!glIsRenderbufferEXT || !glBindRenderbufferEXT || !glDeleteRenderbuffersEXT ||
        !glGenRenderbuffersEXT || !glRenderbufferStorageEXT || !glGetRenderbufferParameterivEXT ||
        !glIsFramebufferEXT || !glBindFramebufferEXT || !glDeleteFramebuffersEXT ||
        !glGenFramebuffersEXT || !glCheckFramebufferStatusEXT || !glFramebufferTexture1DEXT ||
        !glFramebufferTexture2DEXT || !glFramebufferTexture3DEXT || !glFramebufferRenderbufferEXT ||
        !glGetFramebufferAttachmentParameterivEXT || !glGenerateMipmapEXT)
    {
        std::cout << "Entry Point Failure.  :-(" << std::endl;
        return;
    }

    std::cout << "Found.  :-)" << std::endl;

    glFramebufferObjectSupported = GL_TRUE;
    glFramebufferObjectEnabled   = GL_TRUE;
}


static void opengl_arb_shader_objects(void)
{
    std::cout << "Checking for GLEXT:shader_objects...";

    if(!glVersion15 && !strstr(reinterpret_cast<const char*>(glExtensions), "GL_ARB_shader_objects"))
    {
        std::cout << "Not Found.  :-(" << std::endl;
        return;
    }

    glDeleteObjectARB         = reinterpret_cast<PFNGLDELETEOBJECTARBPROC>
                                (SDL_GL_GetProcAddress("glDeleteObjectARB"));
    glGetHandleARB            =  reinterpret_cast<PFNGLGETHANDLEARBPROC>
                                (SDL_GL_GetProcAddress("glGetHandleARB"));
    glDetachObjectARB         = reinterpret_cast<PFNGLDETACHOBJECTARBPROC>
                                (SDL_GL_GetProcAddress("glDetachObjectARB"));
    glCreateShaderObjectARB   = reinterpret_cast<PFNGLCREATESHADEROBJECTARBPROC>
                                (SDL_GL_GetProcAddress("glCreateShaderObjectARB"));
    glShaderSourceARB         = reinterpret_cast<PFNGLSHADERSOURCEARBPROC>
                                (SDL_GL_GetProcAddress("glShaderSourceARB"));
    glCompileShaderARB        = reinterpret_cast<PFNGLCOMPILESHADERARBPROC>
                                (SDL_GL_GetProcAddress("glCompileShaderARB"));
    glCreateProgramObjectARB  = reinterpret_cast<PFNGLCREATEPROGRAMOBJECTARBPROC>
                                (SDL_GL_GetProcAddress("glCreateProgramObjectARB"));
    glAttachObjectARB         = reinterpret_cast<PFNGLATTACHOBJECTARBPROC>
                                (SDL_GL_GetProcAddress("glAttachObjectARB"));
    glLinkProgramARB          = reinterpret_cast<PFNGLLINKPROGRAMARBPROC>
                                (SDL_GL_GetProcAddress("glLinkProgramARB"));
    glUseProgramObjectARB     = reinterpret_cast<PFNGLUSEPROGRAMOBJECTARBPROC>
                                (SDL_GL_GetProcAddress("glUseProgramObjectARB"));
    glValidateProgramARB      = reinterpret_cast<PFNGLVALIDATEPROGRAMARBPROC>
                                (SDL_GL_GetProcAddress("glValidateProgramARB"));
    glUniform1fARB            = reinterpret_cast<PFNGLUNIFORM1FARBPROC>
                                (SDL_GL_GetProcAddress("glUniform1fARB"));
    glUniform2fARB            = reinterpret_cast<PFNGLUNIFORM2FARBPROC>
                                (SDL_GL_GetProcAddress("glUniform2fARB"));
    glUniform3fARB            = reinterpret_cast<PFNGLUNIFORM3FARBPROC>
                                (SDL_GL_GetProcAddress("glUniform3fARB"));
    glUniform4fARB            = reinterpret_cast<PFNGLUNIFORM4FARBPROC>
                                (SDL_GL_GetProcAddress("glUniform4fARB"));
    glUniform1iARB            = reinterpret_cast<PFNGLUNIFORM1IARBPROC>
                                (SDL_GL_GetProcAddress("glUniform1iARB"));
    glUniform2iARB            = reinterpret_cast<PFNGLUNIFORM2IARBPROC>
                                (SDL_GL_GetProcAddress("glUniform2iARB"));
    glUniform3iARB            = reinterpret_cast<PFNGLUNIFORM3IARBPROC>
                                (SDL_GL_GetProcAddress("glUniform3iARB"));
    glUniform4iARB            = reinterpret_cast<PFNGLUNIFORM4IARBPROC>
                                (SDL_GL_GetProcAddress("glUniform4iARB"));
    glUniform1fvARB           = reinterpret_cast<PFNGLUNIFORM1FVARBPROC>
                                (SDL_GL_GetProcAddress("glUniform1fvARB"));
    glUniform2fvARB           = reinterpret_cast<PFNGLUNIFORM2FVARBPROC>
                                (SDL_GL_GetProcAddress("glUniform2fvARB"));
    glUniform3fvARB           = reinterpret_cast<PFNGLUNIFORM3FVARBPROC>
                                (SDL_GL_GetProcAddress("glUniform3fvARB"));
    glUniform4fvARB           = reinterpret_cast<PFNGLUNIFORM4FVARBPROC>
                                (SDL_GL_GetProcAddress("glUniform4fvARB"));
    glUniform1ivARB           = reinterpret_cast<PFNGLUNIFORM1IVARBPROC>
                                (SDL_GL_GetProcAddress("glUniform1ivARB"));
    glUniform2ivARB           = reinterpret_cast<PFNGLUNIFORM2IVARBPROC>
                                (SDL_GL_GetProcAddress("glUniform2ivARB"));
    glUniform3ivARB           = reinterpret_cast<PFNGLUNIFORM3IVARBPROC>
                                (SDL_GL_GetProcAddress("glUniform3ivARB"));
    glUniform4ivARB           = reinterpret_cast<PFNGLUNIFORM4IVARBPROC>
                                (SDL_GL_GetProcAddress("glUniform4ivARB"));
    glUniformMatrix2fvARB     = reinterpret_cast<PFNGLUNIFORMMATRIX2FVARBPROC>
                                (SDL_GL_GetProcAddress("glUniformMatrix2fvARB"));
    glUniformMatrix3fvARB     = reinterpret_cast<PFNGLUNIFORMMATRIX3FVARBPROC>
                                (SDL_GL_GetProcAddress("glUniformMatrix3fvARB"));
    glUniformMatrix4fvARB     = reinterpret_cast<PFNGLUNIFORMMATRIX4FVARBPROC>
                                (SDL_GL_GetProcAddress("glUniformMatrix4fvARB"));
    glGetObjectParameterfvARB = reinterpret_cast<PFNGLGETOBJECTPARAMETERFVARBPROC>
                                (SDL_GL_GetProcAddress("glGetObjectParameterfvARB"));
    glGetObjectParameterivARB = reinterpret_cast<PFNGLGETOBJECTPARAMETERIVARBPROC>
                                (SDL_GL_GetProcAddress("glGetObjectParameterivARB"));
    glGetInfoLogARB           = reinterpret_cast<PFNGLGETINFOLOGARBPROC>
                                (SDL_GL_GetProcAddress("glGetInfoLogARB"));
    glGetAttachedObjectsARB   = reinterpret_cast<PFNGLGETATTACHEDOBJECTSARBPROC>
                                (SDL_GL_GetProcAddress("glGetAttachedObjectsARB"));
    glGetUniformLocationARB   = reinterpret_cast<PFNGLGETUNIFORMLOCATIONARBPROC>
                                (SDL_GL_GetProcAddress("glGetUniformLocationARB"));
    glGetActiveUniformARB     = reinterpret_cast<PFNGLGETACTIVEUNIFORMARBPROC>
                                (SDL_GL_GetProcAddress("glGetActiveUniformARB"));
    glGetUniformfvARB         = reinterpret_cast<PFNGLGETUNIFORMFVARBPROC>
                                (SDL_GL_GetProcAddress("glGetUniformfvARB"));
    glGetUniformivARB         = reinterpret_cast<PFNGLGETUNIFORMIVARBPROC>
                                (SDL_GL_GetProcAddress("glGetUniformivARB"));
    glGetShaderSourceARB      = reinterpret_cast<PFNGLGETSHADERSOURCEARBPROC>
                                (SDL_GL_GetProcAddress("glGetShaderSourceARB"));

    if (!glDeleteObjectARB || !glGetHandleARB || !glDetachObjectARB || !glCreateShaderObjectARB ||
        !glShaderSourceARB || !glCompileShaderARB || !glCreateProgramObjectARB ||
        !glAttachObjectARB || !glLinkProgramARB || !glUseProgramObjectARB || !glValidateProgramARB ||
        !glUniform1fARB || !glUniform2fARB || !glUniform3fARB || !glUniform4fARB ||
        !glUniform1iARB || !glUniform2iARB || !glUniform3iARB || !glUniform4iARB ||
        !glUniform1fvARB || !glUniform2fvARB || !glUniform3fvARB || !glUniform4fvARB ||
        !glUniform1ivARB || !glUniform2ivARB || !glUniform3ivARB || !glUniform4ivARB ||
        !glUniformMatrix2fvARB || !glUniformMatrix3fvARB || !glUniformMatrix4fvARB ||
        !glGetObjectParameterfvARB || !glGetObjectParameterivARB || !glGetInfoLogARB ||
        !glGetAttachedObjectsARB || !glGetUniformLocationARB || !glGetActiveUniformARB ||
        !glGetUniformfvARB || !glGetUniformivARB || !glGetShaderSourceARB)
    {
        std::cout << "Entry Point Failure.  :-(" << std::endl;
        return;
    }

    std::cout << "Found.  :-)" << std::endl;

    glShaderObjectsSupported = GL_TRUE;
    glShaderObjectsEnabled   = GL_FALSE;
}
