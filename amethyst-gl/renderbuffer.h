#pragma once

/***********************************************************************
 Amethyst-GL
  - OpenGL Render Buffer Handling declaration

 Authors (c):
 2016 Beau V.C. Bellamy (bellamy.beau@gmail.com)
 ***********************************************************************/

#include <memory>
#include <list>

namespace amethyst {
namespace client {

class RenderBuffer
{
  public:
    typedef std::shared_ptr<RenderBuffer> sptr;
    typedef std::weak_ptr<RenderBuffer>   wptr;

    typedef unsigned int BufferHDL;
    typedef unsigned int BufferType;
    typedef unsigned int BufferFormat;
    typedef unsigned int BufferSize;

    RenderBuffer();
    virtual ~RenderBuffer();

    BufferHDL operator ()();

    void setName(const std::string& name);
    const std::string& getName();

    void bind();
    
    void setStorage(const BufferFormat intformat, const BufferSize x, const BufferSize y);

  protected:
    // Buffer Handle
    BufferHDL buffer_;

    // GLenum type
    BufferType type_;

  private:
    std::string name_;
};


} // namespace client
} // namsspace amethyst

