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
#include <iostream>

namespace amethyst {
namespace client {

void printInfoLog(GLuint obj);

ShaderProgram::ShaderProgram()
 : _program_hdl(-1)
{

}

ShaderProgram::ShaderProgram(const std::string &vname, const std::string &fname)
 : _program_hdl(-1)
{
    _program_hdl = load_shader(vname, fname);
}

ShaderProgram::~ShaderProgram()
{
    glDeleteProgram(_program_hdl);
}

int ShaderProgram::GetAttribLocation(const std::string& attrib)
{
    int attrib_hdl = glGetAttribLocation(_program_hdl, attrib.c_str());
    if (attrib_hdl < 0)
    {
      opengl_check_errors("GetAttribLocation: ");
      throw std::runtime_error("glGetAtribLocation failed");
    }
    return attrib_hdl;
}

int ShaderProgram::GetUniformLocation(const std::string& uniform)
{
    int attrib_hdl = glGetUniformLocation(_program_hdl, uniform.c_str());
    if (attrib_hdl < 0)
    {
      opengl_check_errors("GetUnformLocation: ");
      throw std::runtime_error("glGetUniformLocation failed");
    }
    return attrib_hdl;
}

void ShaderProgram::use()
{
    glUseProgram(_program_hdl); 
}
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
	GLsizei length;
        glGetShaderInfoLog(myVertexShader, 1000, &length, infoLog);
        Global.log.add("Error in vertex shader compilation!");
        std::cout << "Info Log (" << length << "): " << infoLog << std::endl;
        throw std::runtime_error(infoLog);
    }

    success = 0;
    glCompileShader(myFragmentShader);
    glGetShaderiv(myFragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        GLchar infoLog[1000];
	GLsizei length;
        glGetShaderInfoLog(myFragmentShader, 1000, &length, infoLog);
        //std::cout << "Error in fragment shader compilation!" << std::endl;
        //std::cout << "Info Log: " << infoLog << std::endl;
        throw std::runtime_error(infoLog);
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

    glDeleteShader(myVertexShader);
    glDeleteShader(myFragmentShader);
    
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
