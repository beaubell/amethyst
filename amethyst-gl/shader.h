#ifndef AMETHYST_CLIENT_SHADER_H
#define AMETHYST_CLIENT_SHADER_H

/***********************************************************************
 Amethyst-GL
  - OpenGL shader class declarations

 Authors (c):
 2016-2016 Beau V.C. Bellamy (beau@bellamy.beau@gmail.com)

 $Revision$
 $LastChangedDate$
 $LastChangedBy$
 ***********************************************************************/

#include <memory>
#include <string>

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


} // namespace client
} // namespace amethyst

#endif //AMETHYST_CLIENT_SHADER_H
