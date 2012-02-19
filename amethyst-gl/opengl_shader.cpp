/***********************************************************************
 Amethyst-GL
  - OpenGL shaders setup and processing function implemenations

 Authors (c):
 2008 Beau V.C. Bellamy (beau@stellarnetservices.net)

 $Revision$
 $LastChangedDate$
 $LastChangedBy$
 ***********************************************************************/

#include "opengl.h"
#include "opengl_shader.h"
#include "global.h"
#include "lib/utility.h"


#include <stdlib.h>
#include <string.h>

namespace amethyst {
namespace client {

void printInfoLog(GLhandleARB obj);

int load_shader(const std::string &vname, const std::string &fname)
{

    std::string   v_path = Global.dir_shaders + vname;
    std::string   f_path = Global.dir_shaders + fname;

    GLhandleARB myVertexShader = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
    GLhandleARB myFragmentShader = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);

    std::string v_source;
    std::string f_source;
    lib::readTextFile(v_path, v_source);
    lib::readTextFile(f_path, f_source);

    const char * v_source_ptr = v_source.c_str();
    const char * f_source_ptr = f_source.c_str();
    
    glShaderSourceARB(myVertexShader, 1, &(v_source_ptr), NULL);
    glShaderSourceARB(myFragmentShader, 1, &(f_source_ptr), NULL);

    glCompileShaderARB(myVertexShader);

    GLint success;
    glGetObjectParameterivARB(myVertexShader, GL_OBJECT_COMPILE_STATUS_ARB, &success);
    if (!success)
    {
        GLcharARB infoLog[1000];
        glGetInfoLogARB(myVertexShader, 1000, NULL, infoLog);
        std::cout << "Error in vertex shader compilation!" << std::endl;
        std::cout << "Info Log: " << infoLog << std::endl;
        throw std::runtime_error("Vertex shader failed to compile.");
    }

    success = 0;
    glCompileShaderARB(myFragmentShader);
    glGetObjectParameterivARB(myFragmentShader, GL_OBJECT_COMPILE_STATUS_ARB, &success);
    if (!success)
    {
        GLcharARB infoLog[1000];
        glGetInfoLogARB(myFragmentShader, 1000, NULL, infoLog);
        std::cout << "Error in fragment shader compilation!" << std::endl;
        std::cout << "Info Log: " << infoLog << std::endl;
        throw std::runtime_error("Fragment shader failed to compile.");
    }


    Global.shaderProgram = glCreateProgramObjectARB();
    glAttachObjectARB(Global.shaderProgram, myFragmentShader);
    glAttachObjectARB(Global.shaderProgram, myVertexShader);

    success = 0;

    glLinkProgramARB(Global.shaderProgram);
    glGetObjectParameterivARB(Global.shaderProgram, GL_OBJECT_LINK_STATUS_ARB, &success);
    if (!success)
    {
        printInfoLog(Global.shaderProgram);
    }

    success = 0;

    glValidateProgramARB(Global.shaderProgram);
    glGetObjectParameterivARB(Global.shaderProgram, GL_OBJECT_VALIDATE_STATUS_ARB, &success);
    if (!success)
    {
        GLcharARB infoLog[1000];
        glGetInfoLogARB(Global.shaderProgram, 1000, NULL, infoLog);
        std::cout << "Error in program validation!" << std::endl;
        std::cout << "Info Log: " << infoLog << std::endl;
        //return;
    }

    return Global.shaderProgram;
}


void printInfoLog(GLhandleARB obj)
{
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;

    glGetObjectParameterivARB(obj, GL_OBJECT_INFO_LOG_LENGTH_ARB,
                                            &infologLength);

    if (infologLength > 0)
    {
        infoLog = reinterpret_cast<char *>(malloc(infologLength));
        glGetInfoLogARB(obj, infologLength, &charsWritten, infoLog);
                printf("%s\n",infoLog);
        free(infoLog);
    }
}

} // namespace amethyst
} // namespace client
