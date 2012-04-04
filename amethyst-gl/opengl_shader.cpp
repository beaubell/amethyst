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
#include <stdexcept>

namespace amethyst {
namespace client {

void printInfoLog(GLuint obj);

unsigned int load_shader(const std::string &vname, const std::string &fname)
{

    std::string   v_path = Global.dir_shaders + vname;
    std::string   f_path = Global.dir_shaders + fname;

    GLuint myVertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint myFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    std::string v_source;
    std::string f_source;
    lib::readTextFile(v_path, v_source);
    lib::readTextFile(f_path, f_source);

    const char * v_source_ptr = v_source.c_str();
    const char * f_source_ptr = f_source.c_str();

    glShaderSource(myVertexShader, 1, &(v_source_ptr), NULL);
    glShaderSource(myFragmentShader, 1, &(f_source_ptr), NULL);

    glCompileShader(myVertexShader);

    GLint success;
    glGetShaderiv(myVertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        GLchar infoLog[1000];
        glGetShaderInfoLog(myVertexShader, 1000, NULL, infoLog);
        std::cout << "Error in vertex shader compilation!" << std::endl;
        std::cout << "Info Log: " << infoLog << std::endl;
        throw std::runtime_error("Vertex shader failed to compile.");
    }

    success = 0;
    glCompileShader(myFragmentShader);
    glGetShaderiv(myFragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        GLchar infoLog[1000];
        glGetShaderInfoLog(myFragmentShader, 1000, NULL, infoLog);
        std::cout << "Error in fragment shader compilation!" << std::endl;
        std::cout << "Info Log: " << infoLog << std::endl;
        throw std::runtime_error("Fragment shader failed to compile.");
    }


    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, myFragmentShader);
    glAttachShader(shaderProgram, myVertexShader);

    success = 0;

    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        printInfoLog(shaderProgram);
    }

    success = 0;

    return shaderProgram;
}


void printInfoLog(GLuint obj)
{
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;

    glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &infologLength);

    if (infologLength > 0)
    {
        infoLog = reinterpret_cast<char *>(malloc(infologLength));
        glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
                printf("%s\n",infoLog);
        free(infoLog);
    }
}

} // namespace amethyst
} // namespace client
