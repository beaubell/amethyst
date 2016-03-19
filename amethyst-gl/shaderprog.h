#ifndef AMETHYST_CLIENT_OPENGL_SHADER_H
#define AMETHYST_CLIENT_OPENGL_SHADER_H

/***********************************************************************
 Amethyst-GL
  - OpenGL shaders setup and processing function declarations

 Authors (c):
 2008 Beau V.C. Bellamy (beau@stellarnetservices.net)

 $Revision$
 $LastChangedDate$
 $LastChangedBy$
 ***********************************************************************/

#include "shader.h"

#include "glm/glm.hpp"

#include <string>
#include <memory>

namespace amethyst {
namespace client {


class ShaderProgram
{
public:
  typedef std::shared_ptr<ShaderProgram> sptr;
  typedef std::weak_ptr<ShaderProgram>   wptr;

  class AttribHDL {
  public:
    int value;
    AttribHDL():value(-1){};
    AttribHDL(int v):value(v){};
  };
  class UniformHDL {
  public:
    int value;
    UniformHDL():value(-1){};
    UniformHDL(int v):value(v){};
  };
  
  ShaderProgram();
  ShaderProgram(const std::string &vname, const std::string &fname);
  ~ShaderProgram();
  AttribHDL  GetAttribLocation(const std::string &attrib);
  UniformHDL GetUniformLocation(const std::string &uniform);
  void       UniformMatrix4f(UniformHDL hdl, const glm::mat4 &mat4in);
  void       UniformMatrix3f(UniformHDL hdl, const glm::mat3 &mat3in);
  void       Uniform3f(UniformHDL hdl, const glm::vec3 &vec3in);
  void       Uniform4f(UniformHDL hdl, const glm::vec4 &vec4in);
  void       Uniform1i(UniformHDL hdl, const int iin);
  void       Uniform1f(UniformHDL hdl, const float fin);
  void use();
  int  getHandle();

private:
  int _program_hdl;
  ShaderVertex::sptr      vert_shdr_;
  ShaderFragment::sptr    frag_shdr_;
  ShaderGeometry::sptr    geo_shdr_;
  ShaderTessControl::sptr tessctl_shdr_;
  ShaderTessEval::sptr    tessevl_shdr_;
  ShaderCompute::sptr     comp_shdr;
};

class ScopedUse
{
  public:
    ScopedUse(ShaderProgram::sptr);
   ~ScopedUse();
    ShaderProgram::sptr programPtr;
    int oldProgram;
};


int GetActiveShader();
//unsigned int load_shader(const std::string &vname, const std::string &fname);
void switch_shader(int num);

void printInfoLog(unsigned int obj);

} // namespace amethyst
} // namespace client

#endif // AMETHYST_CLIENT_OPENGL_SHADER_H
