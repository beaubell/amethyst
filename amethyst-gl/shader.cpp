/***********************************************************************
 Amethyst-GL
  - OpenGL shader handler implementations

 Authors (c):
 2016-2020 Beau V.C. Bellamy (beau@bellamy.beau@gmail.com)
 ***********************************************************************/

#include "opengl.h"
#include "shader.h"
#include "global.h"
#include "lib/utility.h"

#include <iostream> //for std::cout

namespace amethyst {
namespace client {

Shader::Shader(const Resource& shader_res, unsigned int  shdr_hdl)
: shdr_(shdr_hdl),
  name_(shader_res.name()),
  source_(shader_res.to_str())
{

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


unsigned int  Shader::operator()()
{
    return shdr_;
}


ShaderVertex::ShaderVertex(const Resource& shader_res)
: Shader(shader_res, glCreateShader(GL_VERTEX_SHADER))
{
}

ShaderFragment::ShaderFragment(const Resource& shader_res)
: Shader(shader_res, glCreateShader(GL_FRAGMENT_SHADER))
{
}

ShaderGeometry::ShaderGeometry(const Resource& shader_res)
: Shader(shader_res, glCreateShader(GL_GEOMETRY_SHADER))
{
}

ShaderTessControl::ShaderTessControl(const Resource& shader_res)
: Shader(shader_res, glCreateShader(GL_TESS_CONTROL_SHADER))
{
}

ShaderTessEval::ShaderTessEval(const Resource& shader_res)
: Shader(shader_res, glCreateShader(GL_TESS_EVALUATION_SHADER))
{
}

ShaderCompute::ShaderCompute(const Resource& shader_res)
: Shader(shader_res, glCreateShader(GL_COMPUTE_SHADER))
{
}


} // namespace lib
} // namespace amethyst
