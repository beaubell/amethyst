/***********************************************************************
 Amethyst-GL
  - OpenGL Buffer Object Handling implementations

 Authors (c):
 2016 Beau V.C. Bellamy (bellamy.beau@gmail.com)
 ***********************************************************************/

#include "bufferobj.h"

#include "opengl.h"

namespace amethyst {
namespace client {

#define DSA 0 //FIXME

//
//
BufferObject::BufferObject(const BufferType& new_type)
 : type_(new_type)
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


void BufferObject::setName(const std::string& name)
{
    name_ = name;
}


const std::string& BufferObject::getName()
{
    return name_;
}


void BufferObject::bind()
{
    glBindBuffer(type_, buffer_);
}

void BufferObject::data(BufferSize size, const void* data, BufferUsage usage)
{
#if DSA
    glNamedBufferData(buffer_, size, data, usage);
#else
    bind();
    glBufferData(type_, size, data, usage);
#endif
}


void BufferObject::enableVertexAttribArray(unsigned int index)
{
#if DSA
    //FIXME
#else
    bind();
    glEnableVertexAttribArray(index);
#endif
}


void BufferObject::vertexAttribPointer(unsigned int index, int size, unsigned int type, bool normalized, unsigned int stride, const void * pointer)
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
VertexArray::VertexArray()
{
    glGenVertexArrays(1, &vao_);
}

VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &vao_);
}

void VertexArray::bind()
{
    glBindVertexArray(vao_);
}

void VertexArray::addBuffer(BufferObject::sptr buffer)
{
    buffers_.push_back(buffer);
    bind();
    buffer->bind();
}

} // namespace client
} // namespace amethyst
