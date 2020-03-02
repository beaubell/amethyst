#pragma once

/***********************************************************************
 Amethyst-GL
  - OpenGL Primitive Object Handling declaration

 Authors (c):
 2016 Beau V.C. Bellamy (bellamy.beau@gmail.com)
 ***********************************************************************/

#include "texture.h"

#include "lib/types.h"        //TransMatrix
#include "shaderprog_model.h"
#include "bufferobj.h"

#include <string>
#include <vector>

namespace amethyst {
namespace client {

class Primitive
{
  public:
    typedef std::shared_ptr<Primitive> sptr;
    typedef std::weak_ptr<Primitive>   wptr;

    typedef glm::vec3 vertex_type;
    typedef glm::vec2 texcoord_type;
    typedef glm::vec3 normal_type;

    Primitive(const std::string& name);
    virtual ~Primitive();
    virtual void render(const TransMatrix& m_proj, const TransMatrix& m_view, const TransMatrix& m_model) = 0;
    virtual void bind(ShaderProgramModel::sptr shaderprog) = 0;
    virtual void clear() = 0;

    void setName(const std::string& name);
    const std::string& getName();

    void addVertex(const vertex_type &vertex, const texcoord_type &texcoord, const normal_type normal);
    void setMaterial(MaterialInfo& matinfo);

  protected:
    ShaderProgramModel::sptr _shader;

    std::vector<vertex_type> _vertii;
    std::vector<texcoord_type> _texcoords;
    std::vector<normal_type> _normals;

    MaterialInfo matinfo_;
    // VAO Buffer Objects
    VertexArray vao_; // Vertex Array Objects Identifier
    ArrayBuffer buffer_[3]; // 0=quads, 1=textures, 2=normals

  private:
    std::string name_;
};


class Triangles : public Primitive
{
  public:
    typedef std::shared_ptr<Triangles> sptr;
    typedef std::weak_ptr<Triangles>   wptr;

    Triangles();
    Triangles(const std::string& name, Texture::sptr texturein);
    virtual ~Triangles();
    virtual void render(const TransMatrix& m_proj, const TransMatrix& m_view, const TransMatrix& m_model);
    virtual void bind(ShaderProgramModel::sptr shaderprog);
    virtual void clear();

    void setTexture(Texture::sptr texturein);

  private:
    Texture::sptr  _texture;
    //uint _vertcount;
};


class TriangleStrip : public Primitive
{
  public:
    typedef std::shared_ptr<TriangleStrip> sptr;
    typedef std::weak_ptr<TriangleStrip>   wptr;

    TriangleStrip();
    TriangleStrip(const std::string& name, Texture::sptr texturein);
    virtual ~TriangleStrip();
    virtual void render(const TransMatrix& m_proj, const TransMatrix& m_view, const TransMatrix& m_model);
    virtual void bind(ShaderProgramModel::sptr shaderprog);
    virtual void clear();

    void setTexture(Texture::sptr texturein);

  private:
    Texture::sptr  _texture;
    //uint _vertcount;
};


} // namespace client
} // namsspace amethyst

