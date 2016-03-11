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

Shader::Shader(const std::string& new_shdr, uint shdr_hdl)
: shdr_(shdr_hdl),
  name_(new_shdr)
{
    std::string path = Global.dir_shaders + new_shdr;

    lib::readTextFile(path, source_);

    const char * source_ptr = source_.c_str();

    glShaderSource(shdr_, 1, &(source_ptr), NULL);

    glCompileShader(shdr_);

    GLint success;
    glGetShaderiv(shdr_, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        GLchar infoLog[1000];
	GLsizei length;
        glGetShaderInfoLog(shdr_, 1000, &length, infoLog);
        Global.log.add("Error in shader compilation!");
        std::cout << "Info Log (" << length << "): " << infoLog << std::endl;
	glDeleteShader(shdr_);
        throw std::runtime_error(infoLog);
    }

}


Shader::~Shader()
{
    glDeleteShader(shdr_);
}


const std::string& Shader::getName()
{
    return name_;
}


uint Shader::operator()()
{
    return shdr_;
}


ShaderVertex::ShaderVertex(const std::string& new_shdr)
 : Shader(new_shdr, glCreateShader(GL_VERTEX_SHADER))
{
}

ShaderFragment::ShaderFragment(const std::string& new_shdr)
 : Shader(new_shdr, glCreateShader(GL_FRAGMENT_SHADER))
{
}

ShaderGeometry::ShaderGeometry(const std::string& new_shdr)
 : Shader(new_shdr, glCreateShader(GL_GEOMETRY_SHADER))
{
}

ShaderTessControl::ShaderTessControl(const std::string& new_shdr)
 : Shader(new_shdr, glCreateShader(GL_TESS_CONTROL_SHADER))
{
}

ShaderTessEval::ShaderTessEval(const std::string& new_shdr)
 : Shader(new_shdr, glCreateShader(GL_TESS_EVALUATION_SHADER))
{
}

ShaderCompute::ShaderCompute(const std::string& new_shdr)
 : Shader(new_shdr, glCreateShader(GL_COMPUTE_SHADER))
{
}


ShaderProgram::ShaderProgram()
 : _program_hdl(-1)
{

}

ShaderProgram::ShaderProgram(const std::string &vname, const std::string &fname)
 : _program_hdl(glCreateProgram())
{
    //_program_hdl = load_shader(vname, fname);
    try
    {
        vert_shdr_ = std::make_shared<ShaderVertex>(vname);
        frag_shdr_ = std::make_shared<ShaderFragment>(fname);
	
	glAttachShader(_program_hdl, (*vert_shdr_)());
	glAttachShader(_program_hdl, (*frag_shdr_)());

	GLint success = 0;
	glLinkProgram(_program_hdl);
	glGetProgramiv(_program_hdl, GL_LINK_STATUS, &success);
	if (!success)
	{
	    GLchar infoLog[1000];
	    GLsizei length;
	    glGetProgramInfoLog(_program_hdl, 1000, &length, infoLog);
	    //printInfoLog(_program_hdl);
	    throw std::runtime_error(infoLog);
	}
    
    }
    catch (...)
    {
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
