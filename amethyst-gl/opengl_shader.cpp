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

#include <glm/gtc/type_ptr.hpp>

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

ShaderProgram::AttribHDL ShaderProgram::GetAttribLocation(const std::string& attrib)
{
    int hdl = glGetAttribLocation(_program_hdl, attrib.c_str());
    if (hdl < 0)
    {
      opengl_check_errors("GetAttribLocation: ");
      throw std::runtime_error("glGetAtribLocation failed: " + attrib + " for **FIXME**" );
    }
    return AttribHDL(hdl);
}

ShaderProgram::UniformHDL ShaderProgram::GetUniformLocation(const std::string& uniform)
{
    int attrib_hdl = glGetUniformLocation(_program_hdl, uniform.c_str());
    if (attrib_hdl < 0)
    {
      opengl_check_errors("GetUnformLocation: ");
      throw std::runtime_error("glGetUniformLocation failed" + uniform + " for **FIXME**"  );
    }
    return attrib_hdl;
}

void ShaderProgram::UniformMatrix4f(ShaderProgram::UniformHDL hdl, const glm::mat4 &mat4in)
{
    if(GetActiveShader() != _program_hdl)
    {
        Global.log.add("Shader not bound during UniformMatrix4f, dumbass.");
	throw std::runtime_error("Shader not bound during UniformMatrix4f");
        //glUseProgram(_program_hdl);
    }
    glUniformMatrix4fv(hdl.value,  1, false, glm::value_ptr(mat4in));
}

void ShaderProgram::Uniform4f(UniformHDL hdl, const glm::vec4& vec4in)
{
    if(GetActiveShader() != _program_hdl)
    {
        Global.log.add("Shader not bound during Uniform4f, dumbass.");
	throw std::runtime_error("Shader not bound during UniformMatrix4f");
        //glUseProgram(_program_hdl);
    }
    glUniform4fv(hdl.value, 1, glm::value_ptr(vec4in));
}

void ShaderProgram::Uniform1i(UniformHDL hdl, const int iin)
{
    if(GetActiveShader() != _program_hdl)
    {
        Global.log.add("Shader not bound during Uniform4f, dumbass.");
	throw std::runtime_error("Shader not bound during UniformMatrix4f");
        //glUseProgram(_program_hdl);
    }
    glUniform1i(hdl.value, iin);
}


void ShaderProgram::use()
{
    glUseProgram(_program_hdl); 
}

int ShaderProgram::getHandle()
{
    return _program_hdl;
}

ScopedUse::ScopedUse(ShaderProgram::sptr sptr)
 : programPtr(sptr)
{
    glGetIntegerv(GL_CURRENT_PROGRAM, &oldProgram);
    sptr->use();
}

ScopedUse::~ScopedUse()
{
    glUseProgram(oldProgram);
}

int GetActiveShader()
{
    int actProgram = -1;
    glGetIntegerv(GL_CURRENT_PROGRAM, &actProgram);
    return actProgram;
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
        std::cout << "Error in fragment shader compilation!" << std::endl;
        std::cout << "Info Log: " << infoLog << std::endl;
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
        GLchar infoLog[1000];
	GLsizei length;
	glGetProgramInfoLog(shaderProgram, 1000, &length, infoLog);
        //printInfoLog(shaderProgram);
	throw std::runtime_error(infoLog);
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
