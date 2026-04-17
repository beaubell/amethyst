/***********************************************************************
 Amethyst-GL
  - OpenGL Buffer Object Handling implementations

 Authors (c):
 2026 Beau V.C. Bellamy (bellamy.beau@gmail.com)
 ***********************************************************************/

#include "bufferobj.h"
#include "opengl.h"

#include <limits>



namespace amethyst::client {

#define DSA 0 //FIXME

//
//
BufferObject::BufferObject(const BufferType& new_type)
 : buffer_(std::numeric_limits<decltype(buffer_)>::max()),
   type_(new_type)
{
#if DSA
    glCreateBuffers(1, &buffer_);
#else
    glGenBuffers(1, &buffer_);
#endif
}


BufferObject::~BufferObject()
{
    glDeleteBuffers(1, &buffer_);
}


BufferObject::BufferHDL BufferObject::operator ()() const
{
    return buffer_;  
}


void BufferObject::setName(const std::string& name)
{
    name_ = name;
}


const std::string& BufferObject::getName()
{
    return name_;
}


void BufferObject::bind() const
{
    glBindBuffer(type_, buffer_);
}

void BufferObject::data(BufferSize size, const void* data, BufferUsage usage) const
{
#if DSA
    glNamedBufferData(buffer_, size, data, usage);
#else
    bind();
    glBufferData(type_, size, data, usage);
#endif
}


void BufferObject::enableVertexAttribArray(const unsigned int index) const
{
#if DSA
    //FIXME
#else
    bind();
    glEnableVertexAttribArray(index);
#endif
}


void BufferObject::vertexAttribPointer(const unsigned int index,
                                       const int size,
                                       const unsigned int type,
                                       const bool normalized,
                                       const int stride,
                                       const void * pointer) const
{
#if DSA
    //FIXME
#else
    bind();
    glVertexAttribPointer(index, size, type, normalized, stride, pointer);
#endif
}



//
//
ArrayBuffer::ArrayBuffer()
 : BufferObject(GL_ARRAY_BUFFER)
{
}



//
//
ElementArrayBuffer::ElementArrayBuffer()
 : BufferObject(GL_ELEMENT_ARRAY_BUFFER)
{
}



//
//
VertexArray::VertexArray()
 : vao_(std::numeric_limits<decltype(vao_)>::max())
{
    glGenVertexArrays(1, &vao_);
}

VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &vao_);
}

VertexArray::ArrayHDL VertexArray::operator ()() const
{
    return vao_;  
}

void VertexArray::bind() const
{
    glBindVertexArray(vao_);
}

void VertexArray::addBuffer(const BufferObject::sptr& buffer)
{
    buffers_.push_back(buffer);
    bind();
    buffer->bind();
}

} // namespace amethyst::client
