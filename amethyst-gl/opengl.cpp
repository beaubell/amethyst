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

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <boost/lexical_cast.hpp>
#include <GL/glcorearb.h>
//#include <GL/glew.h>

#include <execinfo.h>
#include <stdio.h>

namespace amethyst {
namespace client {

GLubyte const *glRenderer   = NULL;
GLubyte const *glVendor     = NULL;
GLubyte const *glVersion    = NULL;
GLubyte const *glExtensions = NULL;

void GLDebugCallBack (GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);

void opengl_setup(void)
{
    //
    std::cout << "Initializing OpenGL (Part 2) GLEW..." << std::endl;
    //FIXME glewInit(); //Throwing errors
    
    // Get Basic Info About the OpenGL Context
    glRenderer   = glGetString(GL_RENDERER);
    glVendor     = glGetString(GL_VENDOR);
    glVersion    = glGetString(GL_VERSION);
    //glExtensions = glGetString(GL_EXTENSIONS); //FIXME This seems to be broke in mesa 11.1.1
    std::cout << "GL_RENDERER   = " << glRenderer << std::endl;
    std::cout << "GL_VERSION    = " << glVersion << std::endl;
    std::cout << "GL_VENDOR     = " << glVendor << std::endl;
    //std::cout << "GL_EXTENSIONS = " << glExtensions << std::endl;  

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

    //Register Debug Callbacl
    if(glDebugMessageCallback)
    {
      glDebugMessageCallback((GLDEBUGPROC)GLDebugCallBack, NULL);
      std::cout << "Registered OpenGL Debug Callback." << std::endl;
      
    } else
    {
      std::cout << "OpenGL Debug Callback Not Supported." << std::endl;
      
    }

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

    Global.screen_x = w;
    Global.screen_y = h;

    // Black background
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f );

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glFrontFace(GL_CCW);

    glDepthFunc(GL_LEQUAL);
    glDepthRange(-1.0,1e10);
    //glPolygonOffset(1.0,1.0);
}

int opengl_check_errors(const std::string &where)
{
  int count = 0;
  while (int err = glGetError())
  {
      count++;
      std::cout  <<  "++++" << where << ": GLError Reported: " << err << std::endl;
  }
  return count;
}

void GLDebugCallBack (GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
    std::string logmessage = boost::lexical_cast<std::string>(source);
    Global.log.add(logmessage + std::string(message));
  
    void *array[10];
    size_t size;

    // get void*'s for all entries on the stack
    //size = backtrace(array, 10);

    // print out all the frames to stderr
    //fprintf(stderr, "Error: signal %d:\n", sig);
    //backtrace_symbols_fd(array, size, STDERR_FILENO);
    //exit(1);
    //throw std::runtime_error(logmessage);
}



} // namespace amethyst
} // namespace client
