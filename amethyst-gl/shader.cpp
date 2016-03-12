/***********************************************************************
 Amethyst-GL
  - ??? FIXME ??? implementations

 Authors (c):
 2016-2016 Beau V.C. Bellamy (beau@bellamy.beau@gmail.com)

 $Revision$
 $LastChangedDate$
 $LastChangedBy$
 ***********************************************************************/

#include "opengl.h"
#include "shader.h"
#include "global.h"
#include "lib/utility.h"

#include <iostream> //for std::cout

namespace amethyst {
namespace client {

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


} // namespace lib
} // namespace amethyst
