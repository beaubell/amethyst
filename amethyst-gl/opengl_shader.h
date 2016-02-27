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

#include <string>

#include <boost/shared_ptr.hpp>

#include "glm/glm.hpp"


namespace amethyst {
namespace client {
  
class ShaderProgram
{
public:
  typedef boost::shared_ptr<ShaderProgram> ptr;
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
  void       Uniform4f(UniformHDL hdl, const glm::vec4 &vec4in);
  void use();
  int  getHandle();
  
private:
  int _program_hdl;
};

int GetActiveShader();
unsigned int load_shader(const std::string &vname, const std::string &fname);
void switch_shader(int num);

void printInfoLog(unsigned int obj);

} // namespace amethyst
} // namespace client

#endif // AMETHYST_CLIENT_OPENGL_SHADER_H
