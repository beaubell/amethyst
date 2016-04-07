#pragma once

/***********************************************************************
 Amethyst-GL
  - OpenGL Buffer Object Handling declaration

 Authors (c):
 2016 Beau V.C. Bellamy (bellamy.beau@gmail.com)
 ***********************************************************************/

#include <memory>
#include <list>

namespace amethyst {
namespace client {

class BufferObject
{
  public:
    typedef std::shared_ptr<BufferObject> sptr;
    typedef std::weak_ptr<BufferObject>   wptr;

    typedef unsigned int BufferHDL;
    typedef unsigned int BufferType;
    typedef unsigned int BufferUsage;
    typedef unsigned int BufferSize;

    BufferObject(const BufferType& type);
    virtual ~BufferObject();

    void setName(const std::string& name);
    const std::string& getName();

    void bind();
    void data(BufferSize size, const void* data, BufferUsage usage);
    void enableVertexAttribArray(unsigned int index);
    void vertexAttribPointer(unsigned int index, int size, unsigned int type, bool normalized, unsigned int stride, const void * pointer);

  protected:
    // Buffer Handle
    BufferHDL buffer_;

    // GLenum type
    BufferType type_;

  private:
    std::string name_;
};


class ArrayBuffer : public BufferObject
{
  public:
    typedef std::shared_ptr<ArrayBuffer> sptr;
    typedef std::weak_ptr<ArrayBuffer>   wptr;

    ArrayBuffer();

};



class VertexArray
{
  public:
    typedef std::shared_ptr<VertexArray> sptr;
    typedef std::weak_ptr<VertexArray>   wptr;

    typedef unsigned int ArrayHDL;

    VertexArray();
    virtual ~VertexArray();

    void bind();

    void addBuffer(BufferObject::sptr buffer);

  protected:
    std::list<BufferObject::sptr> buffers_;

    // Buffer Handle
    ArrayHDL vao_;
};

} // namespace client
} // namsspace amethyst

