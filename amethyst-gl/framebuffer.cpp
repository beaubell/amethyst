/***********************************************************************
 Amethyst-GL
  - OpenGL FrameBuffer class implementations

 Authors (c):
 2016-2016 Beau V.C. Bellamy (bellamy.beau@gmail.com)
 ***********************************************************************/

#include "framebuffer.h"
#include "texture.h"
#include "opengl.h"

namespace amethyst {
namespace client {

FrameBuffer::FrameBuffer(const std::string& name)
: type_(GL_FRAMEBUFFER),
  name_(name)
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
    glBindFramebuffer(type_, fb_);
}


void FrameBuffer::attachColor0(RenderBuffer& color0)
{
    glFramebufferRenderbuffer(type_, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, color0());
}


void FrameBuffer::attachColor0(Texture& color0tex, int level)
{
    glFramebufferTexture(type_, GL_COLOR_ATTACHMENT0, color0tex(), level);
}


void FrameBuffer::attachDepth(RenderBuffer& depth)
{
    glFramebufferRenderbuffer(type_, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth());
}


void FrameBuffer::attachDepth(Texture& depthtex, int level)
{
    glFramebufferTexture(type_, GL_DEPTH_ATTACHMENT, depthtex(), level);
}


void FrameBuffer::attachStencil(RenderBuffer& stencil)
{
    glFramebufferRenderbuffer(type_, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, stencil());
}


void FrameBuffer::attachStencil(Texture& stenciltex, int level)
{
    glFramebufferTexture(type_, GL_STENCIL_ATTACHMENT, stenciltex(), level);
}


} // namespace lib
} // namespace amethyst
