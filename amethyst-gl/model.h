#ifndef AMETHYST_CLIENT_MODEL_H
#define AMETHYST_CLIENT_MODEL_H

/***********************************************************************
 Amethyst-GL
  - Model loading and processing function prototypes

 Authors (c):
 2004-2008 Beau V.C. Bellamy (beau@stellarnetservices.net)

 $Revision$
 $LastChangedDate$
 $LastChangedBy$
 ***********************************************************************/

#include "texture.h"

#include "lib/types.h"
#include "opengl_shader.h"

#include <string>
#include <list>
#include <memory>

namespace amethyst {
namespace client {

class Primative
{
  public:
    typedef std::shared_ptr<Primative> sptr;
    typedef std::weak_ptr<Primative>   wptr;

    Primative(const std::string& name);
    virtual ~Primative();
    virtual void render(const TransMatrix& m_proj, const TransMatrix& m_view, const TransMatrix& m_model) = 0;
    virtual void bind(ShaderProgram::sptr shaderprog) = 0;
    virtual void clear() = 0;

    void setName(const std::string& name);
    const std::string& getName();

  private:
    std::string _name;

};

class TriangleStrip : public Primative
{
  public:
    typedef std::shared_ptr<TriangleStrip> sptr;
    typedef std::weak_ptr<TriangleStrip>   wptr;

    typedef glm::vec3 vertex_type;
    typedef glm::vec2 texcoord_type;
    typedef glm::vec3 normal_type;

    TriangleStrip();
    TriangleStrip(const std::string& name, Texture::sptr texturein);
    virtual ~TriangleStrip();
    virtual void render(const TransMatrix& m_proj, const TransMatrix& m_view, const TransMatrix& m_model);
    virtual void bind(ShaderProgram::sptr shaderprog);
    virtual void clear();

    void setTexture(Texture::sptr texturein);
    void addVertex(const vertex_type &vertex, const texcoord_type &texcoord, const normal_type normal);

  private:
    ShaderProgram::sptr _shader;
    Texture::sptr  _texture;
    uint _vertcount;
    std::vector<vertex_type> _vertii;
    std::vector<texcoord_type> _texcoords;
    std::vector<normal_type> _normals;

    ShaderProgram::UniformHDL _mprojloc;
    ShaderProgram::UniformHDL _mviewloc;
    ShaderProgram::UniformHDL _mmodeloc;
    ShaderProgram::UniformHDL _vlightloc;

    ShaderProgram::AttribHDL _vertexloc;
    ShaderProgram::AttribHDL _texcoordloc;
    ShaderProgram::AttribHDL _normalloc;
    ShaderProgram::AttribHDL _texunitloc;
    
    // VAO Buffer Objects
    unsigned int _vao[1]; // Vertex Array Objects Identifier
    unsigned int _buffer[3]; // 0=quads, 1=textures, 2=normals

};

class Model
{
   public:
    typedef std::shared_ptr<Model> sptr;
    typedef std::weak_ptr<Model>   wptr;

    Model();
    virtual ~Model();
    virtual void render(const TransMatrix& m_proj, const TransMatrix& m_view, const TransMatrix& m_model);
    //virtual void clear();

    void setName(const std::string& name);
    const std::string& getName();

    void addPrimative(Primative::sptr newprim);

  private:
    std::string _name;
    std::list<Primative::sptr> _primatives;
};

extern std::list<Model::sptr>  model_list;

void model_add(Model::sptr newmodel);
Model::sptr model_find(const std::string &name);
void models_free(void);

Model::sptr model_load(std::string &model_name);


void model_load_file(const std::string &filename, Model &model, Texture::sptr tex);

TriangleStrip::sptr model_sphere_create(double cx, double cy, double cz, double r, int p );

} // namespace client
} // namsspace amethyst

#endif /* AMETHYST_CLIENT_MODEL_H */
