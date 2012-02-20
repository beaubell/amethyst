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

namespace amethyst {
namespace client {

GLubyte const *glRenderer   = NULL;
GLubyte const *glVendor     = NULL;
GLubyte const *glVersion    = NULL;
GLubyte const *glExtensions = NULL;

void opengl_setup(void)
{
    std::cout << "Initializing OpenGL (Part 2) GLEW..." << std::endl;
    glewInit();

    // Get Basic Info About the OpenGL Context
    glRenderer   = glGetString(GL_RENDERER);
    glVendor     = glGetString(GL_VENDOR);
    glVersion    = glGetString(GL_VERSION);
    glExtensions = glGetString(GL_EXTENSIONS);
    std::cout << "GL_RENDERER   = " << glRenderer << std::endl;
    std::cout << "GL_VERSION    = " << glVersion << std::endl;
    std::cout << "GL_VENDOR     = " << glVendor << std::endl;
    std::cout << "GL_EXTENSIONS = " << glExtensions << std::endl;

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

    GLboolean glstereo;
    glGetBooleanv(GL_STEREO, &glstereo);
    std::cout << "GL_STEREO: " << ((glstereo)?"Supported":"Not Supported") << std::endl;

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

} // namespace amethyst
} // namespace client
