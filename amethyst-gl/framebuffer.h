#pragma once

/***********************************************************************
 Amethyst-GL
  - OpenGL FrameBuffer class declarations

 Authors (c):
 2016-2016 Beau V.C. Bellamy (bellamy.beau@gmail.com)
 ***********************************************************************/

#include "renderbuffer.h"
#include "texture.h"
#include <memory>

namespace amethyst {
namespace client {

class FrameBuffer
{
  public:
    typedef std::shared_ptr<FrameBuffer> sptr;
    typedef std::weak_ptr<FrameBuffer>   wptr;

    typedef unsigned int FrameBufferHDL;
    typedef unsigned int FrameBufferType;

    FrameBuffer(const std::string& name);
    virtual ~FrameBuffer();
    const std::string& getName();

    void bind();
    
    void attachDepth(RenderBuffer& depthfb);
    void attachDepth(Texture& depthtex, int level);
    void attachColor0(RenderBuffer& color0fb);
    void attachColor0(Texture& color0tex, int level);
    void attachStencil(RenderBuffer& stencilfb);
    void attachStencil(Texture& stenciltex, int level);

  protected:
    FrameBufferHDL fb_;
    FrameBufferType type_;
    
  private:
    std::string name_;
};


} // namespace client
} // namespace amethyst
