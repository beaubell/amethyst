#ifndef AMETHYST_CLIENT_SHADER_H
#define AMETHYST_CLIENT_SHADER_H

/***********************************************************************
 Amethyst-GL
  - OpenGL shader class declarations

 Authors (c):
 2016-2020 Beau V.C. Bellamy (beau@bellamy.beau@gmail.com)
 ***********************************************************************/

#include "lib/resource.h"

#include <memory>
#include <string>

namespace amethyst {
namespace client {

class Shader
{
  public:
    typedef std::shared_ptr<Shader> sptr;
    typedef std::weak_ptr<Shader>   wptr;

    Shader(const Resource& shader_res, uint shdr_hdl);
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

    ShaderVertex(const Resource& shader_res);
};

class ShaderFragment : public Shader
{
  public:
    typedef std::shared_ptr<ShaderFragment> sptr;
    typedef std::weak_ptr<ShaderFragment>   wptr;

    ShaderFragment(const Resource& shader_res);
};

class ShaderGeometry : public Shader
{
  public:
    typedef std::shared_ptr<ShaderGeometry> sptr;
    typedef std::weak_ptr<ShaderGeometry>   wptr;

    ShaderGeometry(const Resource& shader_res);
};

class ShaderTessControl : public Shader
{
  public:
    typedef std::shared_ptr<ShaderTessControl> sptr;
    typedef std::weak_ptr<ShaderTessControl>   wptr;

    ShaderTessControl(const Resource& shader_res);
};

class ShaderTessEval : public Shader
{
  public:
    typedef std::shared_ptr<ShaderTessEval> sptr;
    typedef std::weak_ptr<ShaderTessEval>   wptr;

    ShaderTessEval(const Resource& shader_res);
};

class ShaderCompute : public Shader
{
  public:
    typedef std::shared_ptr<ShaderCompute> sptr;
    typedef std::weak_ptr<ShaderCompute>   wptr;

    ShaderCompute(const Resource& shader_res);
};


} // namespace client
} // namespace amethyst

#endif //AMETHYST_CLIENT_SHADER_H
