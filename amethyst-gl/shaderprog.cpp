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
#include "shader.h"
#include "shaderprog.h"
#include "global.h"
#include "lib/utility.h"
#include "lib/resource.h"


#include <stdlib.h>
#include <string.h>
#include <stdexcept>
#include <iostream>

#include <glm/gtc/type_ptr.hpp>

namespace amethyst
{
namespace client
{

void printInfoLog(GLuint obj);


ShaderProgram::ShaderProgram()
    : _program_hdl(-1)
{

}

ShaderProgram::ShaderProgram(const Resource &vs, const Resource &fs)
    : _program_hdl(glCreateProgram())
{

    try {
        vert_shdr_ = std::make_shared<ShaderVertex>(vs);
        frag_shdr_ = std::make_shared<ShaderFragment>(fs);

        glAttachShader(_program_hdl, (*vert_shdr_)());
        glAttachShader(_program_hdl, (*frag_shdr_)());

        GLint success = 0;
        glLinkProgram(_program_hdl);
        glGetProgramiv(_program_hdl, GL_LINK_STATUS, &success);
        if (!success) {
            GLchar infoLog[1000];
            GLsizei length;
            glGetProgramInfoLog(_program_hdl, 1000, &length, infoLog);
            //printInfoLog(_program_hdl);
            throw std::runtime_error(infoLog);
        }

    } catch (...) {
        glDeleteProgram(_program_hdl);
        throw;
    }
}

ShaderProgram::~ShaderProgram()
{
    //FIXME
    //if(_program_hdl != -1)
    //  glDeleteProgram(_program_hdl);
    //
}

ShaderProgram::AttribHDL ShaderProgram::GetAttribLocation(const std::string &attrib)
{
    int hdl = glGetAttribLocation(_program_hdl, attrib.c_str());
    if (hdl < 0) {
        opengl_check_errors("GetAttribLocation: ");
        throw std::runtime_error("glGetAtribLocation failed: " + attrib + " for **FIXME**");
    }
    return AttribHDL(hdl);
}

ShaderProgram::UniformHDL ShaderProgram::GetUniformLocation(const std::string &uniform)
{
    int attrib_hdl = glGetUniformLocation(_program_hdl, uniform.c_str());
    if (attrib_hdl < 0) {
        opengl_check_errors("GetUnformLocation: ");
        throw std::runtime_error("glGetUniformLocation failed" + uniform + " for **FIXME**");
    }
    return attrib_hdl;
}

void ShaderProgram::UniformMatrix4f(ShaderProgram::UniformHDL hdl, const glm::mat4 &mat4in)
{
    if (GetActiveShader() != _program_hdl) {
        Global.log.add("Shader not bound during UniformMatrix4f.");
        throw std::runtime_error("Shader not bound during UniformMatrix4f");
    }
    glUniformMatrix4fv(hdl.value,  1, false, glm::value_ptr(mat4in));
}

void ShaderProgram::UniformMatrix3f(ShaderProgram::UniformHDL hdl, const glm::mat3 &mat3in)
{
    if (GetActiveShader() != _program_hdl) {
        Global.log.add("Shader not bound during UniformMatrix3f.");
        throw std::runtime_error("Shader not bound during UniformMatrix3f");
    }
    glUniformMatrix3fv(hdl.value,  1, false, glm::value_ptr(mat3in));
}

void ShaderProgram::Uniform2f(ShaderProgram::UniformHDL hdl, const glm::vec2 &vec2in)
{
    if (GetActiveShader() != _program_hdl) {
        Global.log.add("Shader not bound during Uniform2f.");
        throw std::runtime_error("Shader not bound during Uniform2f");
    }
    glUniform2fv(hdl.value, 1, glm::value_ptr(vec2in));
}


void ShaderProgram::Uniform3f(UniformHDL hdl, const glm::vec3 &vec3in)
{
    if (GetActiveShader() != _program_hdl) {
        Global.log.add("Shader not bound during Uniform3f.");
        throw std::runtime_error("Shader not bound during Uniform3f");
    }
    glUniform3fv(hdl.value, 1, glm::value_ptr(vec3in));
}

void ShaderProgram::Uniform4f(UniformHDL hdl, const glm::vec4 &vec4in)
{
    if (GetActiveShader() != _program_hdl) {
        Global.log.add("Shader not bound during Uniform4f.");
        throw std::runtime_error("Shader not bound during Uniform4f");
    }
    glUniform4fv(hdl.value, 1, glm::value_ptr(vec4in));
}

void ShaderProgram::Uniform1i(UniformHDL hdl, const int iin)
{
    if (GetActiveShader() != _program_hdl) {
        Global.log.add("Shader not bound during Uniform1i.");
        throw std::runtime_error("Shader not bound during Uniform1i");
    }
    glUniform1i(hdl.value, iin);
}

void ShaderProgram::Uniform1f(UniformHDL hdl, const float fin)
{
    if (GetActiveShader() != _program_hdl) {
        Global.log.add("Shader not bound during Uniform1f.");
        throw std::runtime_error("Shader not bound during Uniform1f");
    }
    glUniform1f(hdl.value, fin);
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


void printInfoLog(GLuint obj)
{
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;

    glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &infologLength);

    if (infologLength > 0) {
        infoLog = reinterpret_cast<char *>(malloc(infologLength));
        glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
        printf("%s\n", infoLog);
        free(infoLog);
    }
}

} // namespace amethyst
} // namespace client
