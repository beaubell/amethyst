#pragma once

/***********************************************************************
 Amethyst-GL
  - OpenGL Buffer Object Handling declaration

 Authors (c):
 2026 Beau V.C. Bellamy (bellamy.beau@gmail.com)
 ***********************************************************************/

#include <memory>
#include <list>
#include <string>

namespace amethyst::client {

class BufferObject
{
  public:
    typedef std::shared_ptr<BufferObject> sptr;
    typedef std::weak_ptr<BufferObject>   wptr;

    typedef unsigned int BufferHDL;
    typedef unsigned int BufferType;
    typedef unsigned int BufferUsage;
    typedef unsigned int BufferSize;

    explicit BufferObject(const BufferType& type);
    virtual ~BufferObject();
    
    BufferHDL operator ()() const;

    void setName(const std::string& name);
    const std::string& getName();

    void bind() const;
    void data(BufferSize size, const void* data, BufferUsage usage) const;
    void enableVertexAttribArray(unsigned int index) const;
    void vertexAttribPointer(unsigned int index, int size, unsigned int type, bool normalized, int stride, const void * pointer) const;

  protected:
    // Buffer Handle
    BufferHDL buffer_;

    // GLenum type
    BufferType type_;

  private:
    std::string name_;
};


class ArrayBuffer final : public BufferObject
{
  public:
    typedef std::shared_ptr<ArrayBuffer> sptr;
    typedef std::weak_ptr<ArrayBuffer>   wptr;

    ArrayBuffer();

};

class ElementArrayBuffer final : public BufferObject
{
  public:
    typedef std::shared_ptr<ElementArrayBuffer> sptr;
    typedef std::weak_ptr<ElementArrayBuffer>   wptr;

    ElementArrayBuffer();

};

class VertexArray final
{
  public:
    typedef std::shared_ptr<VertexArray> sptr;
    typedef std::weak_ptr<VertexArray>   wptr;

    typedef unsigned int ArrayHDL;

    VertexArray();
    virtual ~VertexArray();
    
    ArrayHDL operator ()() const;

    void bind() const;

    void addBuffer(const BufferObject::sptr& buffer);

  protected:
    std::list<BufferObject::sptr> buffers_;

    // Buffer Handle
    ArrayHDL vao_;
};

} // namsspace amethyst::client

