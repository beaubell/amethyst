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

namespace amethyst {
namespace client {

class ShaderProgram
{
public:
  typedef boost::shared_ptr<ShaderProgram> ptr;
  
  ShaderProgram();
  ShaderProgram(const std::string &vname, const std::string &fname);
  ~ShaderProgram();
  int GetAttribLocation(const std::string &attrib);
  int GetUniformLocation(const std::string &uniform);
  void use();
  
private:
  int _program_hdl;
};
  
unsigned int load_shader(const std::string &vname, const std::string &fname);
void switch_shader(int num);

void printInfoLog(unsigned int obj);

} // namespace amethyst
} // namespace client

#endif // AMETHYST_CLIENT_OPENGL_SHADER_H
