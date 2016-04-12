/***********************************************************************
 Amethyst-GL
  - OpenGL Render Buffer Handling implementations

 Authors (c):
 2016 Beau V.C. Bellamy (bellamy.beau@gmail.com)
 ***********************************************************************/

#include "renderbuffer.h"

#include "opengl.h"

namespace amethyst {
namespace client {

#define DSA 0 //FIXME

//
//
RenderBuffer::RenderBuffer()
 : type_(GL_RENDERBUFFER)
{
#if DSA
    glCreateRenderbuffers(1, &buffer_);
#else
    glGenRenderbuffers(1, &buffer_);
#endif
}


RenderBuffer::~RenderBuffer()
{
#if DSA
    //FIXME
#else
    glGenRenderbuffers(1, &buffer_);
#endif
}


RenderBuffer::BufferHDL RenderBuffer::operator ()()
{
    return buffer_;  
}


void RenderBuffer::setName(const std::string& name)
{
    name_ = name;
}


const std::string& RenderBuffer::getName()
{
    return name_;
}


void RenderBuffer::bind()
{
    glBindRenderbuffer(type_, buffer_);
}


void RenderBuffer::setStorage(const RenderBuffer::BufferFormat intformat, const RenderBuffer::BufferSize x, const RenderBuffer::BufferSize y)
{
#if DSA
    //FIXME 
#else
    glRenderbufferStorage(type_, intformat, x, y);
#endif
}


} // namespace client
} // namespace amethyst
