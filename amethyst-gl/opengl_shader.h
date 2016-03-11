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

#include "glm/glm.hpp"

#include <string>
#include <memory>

namespace amethyst {
namespace client {

class Shader
{
  public:
    typedef std::shared_ptr<Shader> sptr;
    typedef std::weak_ptr<Shader>   wptr;

    Shader(const std::string& new_shdr, uint shdr_hdl);
    virtual ~Shader();
    const std::string& getName();

    uint operator()();

  private:
    uint shdr_;
    std::string name_;
    std::string source_;
};

class ShaderVertex : public Shader
{
  public:
  typedef std::shared_ptr<ShaderVertex> sptr;
  typedef std::weak_ptr<ShaderVertex>   wptr;

   ShaderVertex(const std::string& new_shdr);
};

class ShaderFragment : public Shader
{
  public:
  typedef std::shared_ptr<ShaderFragment> sptr;
  typedef std::weak_ptr<ShaderFragment>   wptr;

  ShaderFragment(const std::string& new_shdr);
};

class ShaderGeometry : public Shader
{
  public:
  typedef std::shared_ptr<ShaderGeometry> sptr;
  typedef std::weak_ptr<ShaderGeometry>   wptr;

  ShaderGeometry(const std::string& new_shdr);
};

class ShaderTessControl : public Shader
{
  public:
  typedef std::shared_ptr<ShaderTessControl> sptr;
  typedef std::weak_ptr<ShaderTessControl>   wptr;

  ShaderTessControl(const std::string& new_shdr);
};

class ShaderTessEval : public Shader
{
  public:
  typedef std::shared_ptr<ShaderTessEval> sptr;
  typedef std::weak_ptr<ShaderTessEval>   wptr;

  ShaderTessEval(const std::string& new_shdr);
};

class ShaderCompute : public Shader
{
  public:
  typedef std::shared_ptr<ShaderCompute> sptr;
  typedef std::weak_ptr<ShaderCompute>   wptr;

  ShaderCompute(const std::string& new_shdr);
};

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
  void       Uniform4f(UniformHDL hdl, const glm::vec4 &vec4in);
  void       Uniform1i(UniformHDL hdl, const int iin);
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
