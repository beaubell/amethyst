/***********************************************************************
 Amethyst-GL
  - OpenGL FrameBuffer class implementations

 Authors (c):
 2016-2016 Beau V.C. Bellamy (bellamy.beau@gmail.com)
 ***********************************************************************/

#include "framebuffer.h"
#include "opengl.h"

namespace amethyst {
namespace client {

FrameBuffer::FrameBuffer(const std::string& name)
: name_(name)
{
    glGenFramebuffers(1, &fb_);
}


FrameBuffer::~FrameBuffer()
{
    glDeleteFramebuffers(1, &fb_);
}


const std::string& FrameBuffer::getName()
{
    return name_;
}


void FrameBuffer::bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, fb_);
}

} // namespace lib
} // namespace amethyst
