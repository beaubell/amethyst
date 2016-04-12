#pragma once

/***********************************************************************
 Amethyst-GL
  - OpenGL FrameBuffer class declarations

 Authors (c):
 2016-2016 Beau V.C. Bellamy (bellamy.beau@gmail.com)
 ***********************************************************************/

#include <memory>

namespace amethyst {
namespace client {

class FrameBuffer
{
  public:
    typedef std::shared_ptr<FrameBuffer> sptr;
    typedef std::weak_ptr<FrameBuffer>   wptr;
    
    typedef unsigned int FrameBufferHDL;

    FrameBuffer(const std::string& name);
    virtual ~FrameBuffer();
    const std::string& getName();

    void bind();

  private:
    FrameBufferHDL fb_;
    std::string name_;
};


} // namespace client
} // namespace amethyst
