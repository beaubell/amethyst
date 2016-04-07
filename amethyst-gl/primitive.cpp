/***********************************************************************
 Amethyst-GL
  - OpenGL Primitive Object Handling implementations

 Authors (c):
 2016 Beau V.C. Bellamy (bellamy.beau@gmail.com)
 ***********************************************************************/

#include "primitive.h"

#include "global.h" //FIXME

namespace amethyst {
namespace client {

//
//
Primitive::Primitive(const std::string& name)
 : name_(name)
{}


Primitive::~Primitive()
{}

void Primitive::setName(const std::string& name)
{
    name_ = name;
}

const std::string& Primitive::getName()
{
    return name_;
}

void Primitive::addVertex(const vertex_type &vertex, const texcoord_type &texcoord, const normal_type normal)
{
    _vertii.push_back(vertex);
    _texcoords.push_back(texcoord);
    _normals.push_back(normal);
}

//
//
Triangles::Triangles()
 : Primitive("Triangles")
{}

Triangles::Triangles(const std::string& name, Texture::sptr texturein)
 : Primitive(name),
   _texture(texturein)
{
}

Triangles::~Triangles()
{
}

void Triangles::render(const TransMatrix& m_proj, const TransMatrix& m_view, const TransMatrix& m_model)
{
    if(!_shader)
        throw std::runtime_error("Triangles not bound to shader " + this->getName());
    
    const lib::Cartesian_Vector &reference = Global.obj_view->location;

    LightInfo light;
    light.Position = m_view*m_model*glm::vec4(-reference.x, -reference.y, -reference.z, 1.0f);
    light.Intensity = glm::vec3(1.0f, 1.0f, 1.0f);

    MaterialInfo matinfo;
    matinfo.Ka = glm::vec3(0.0f, 0.0f, 0.0f);
    matinfo.Kd = glm::vec3(1.0f, 1.0f, 1.0f);
    matinfo.Ks = glm::vec3(1.0f, 1.0f, 1.1f);
    matinfo.Shininess = 200.0f;


    _shader->use();
    _shader->setProjM(m_proj);
    _shader->setModelViewM(m_view * m_model);
    _shader->setNormalM(glm::mat3(m_view*m_model));
    _shader->setMVP(m_proj*m_view*m_model);
    _shader->setLight(light);
    _shader->setMaterial(matinfo);

    if(_texture) //FIXME
      _texture->bind();

    // Render vao objects
    vao_.bind();
    glDrawArrays(GL_TRIANGLES, 0, _vertii.size());
}

void Triangles::bind(ShaderProgramModel::sptr shaderprog)
{
    _shader  = shaderprog;

    // Bind Vextex Array Buffer Object to store state for buffers
    vao_.bind();

    // Bind buffer 0 to vao
    buffer_[0].bind();
    buffer_[0].data(_vertii.size()*sizeof(vertex_type), &_vertii[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(_shader->getVertexLoc());
    glVertexAttribPointer(_shader->getVertexLoc(), 3, GL_FLOAT, 0, 0, 0);

    buffer_[1].bind();
    buffer_[1].data(_texcoords.size()*sizeof(texcoord_type), &_texcoords[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(_shader->getTexCoordLoc());
    glVertexAttribPointer(_shader->getTexCoordLoc(), 2, GL_FLOAT, 0, 0, 0);

    buffer_[2].bind();
    buffer_[2].data(_normals.size()*sizeof(normal_type), &_normals[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(_shader->getNormalLoc());
    glVertexAttribPointer(_shader->getNormalLoc(), 3, GL_FLOAT, 0, 0, 0);
}

void Triangles::clear()
{
    _vertii.clear();
    _texcoords.clear();
    _normals.clear();
}

void Triangles::setTexture(Texture::sptr texturein)
{
    _texture = texturein;
}


//
//
TriangleStrip::TriangleStrip()
 : Primitive("TriangleStrip")
{}

TriangleStrip::TriangleStrip(const std::string& name, Texture::sptr texturein)
 : Primitive(name),
   _texture(texturein)
{
}

TriangleStrip::~TriangleStrip()
{
}

void TriangleStrip::render(const TransMatrix& m_proj, const TransMatrix& m_view, const TransMatrix& m_model)
{
    if(!_shader)
        throw std::runtime_error("TriangleStrip not bound to shader " + this->getName());
    
    const lib::Cartesian_Vector &reference = Global.obj_view->location;

    LightInfo light;
    light.Position = m_view*m_model*glm::vec4(-reference.x, -reference.y, -reference.z, 1.0f);
    light.Intensity = glm::vec3(1.0f, 1.0f, 1.0f);

    MaterialInfo matinfo;
    matinfo.Ka = glm::vec3(0.0f, 0.0f, 0.0f);
    matinfo.Kd = glm::vec3(1.0f, 1.0f, 1.0f);
    matinfo.Ks = glm::vec3(1.0f, 1.0f, 1.1f);
    matinfo.Shininess = 200.0f;


    _shader->use();
    _shader->setProjM(m_proj);
    _shader->setModelViewM(m_view * m_model);
    _shader->setNormalM(glm::mat3(m_view*m_model));
    _shader->setMVP(m_proj*m_view*m_model);
    _shader->setLight(light);
    _shader->setMaterial(matinfo);
    _texture->bind();

    // Render vao objects
    vao_.bind();
    glDrawArrays(GL_TRIANGLE_STRIP, 0, _vertii.size());
}

void TriangleStrip::bind(ShaderProgramModel::sptr shaderprog)
{
    _shader  = shaderprog;

    // Bind Vextex Array Buffer Object to store state for buffers
    vao_.bind();

    // Bind buffer 0 to vao
    buffer_[0].bind();
    buffer_[0].data(_vertii.size()*sizeof(vertex_type), &_vertii[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(_shader->getVertexLoc());
    glVertexAttribPointer(_shader->getVertexLoc(), 3, GL_FLOAT, 0, 0, 0);

    buffer_[1].bind();
    buffer_[1].data(_texcoords.size()*sizeof(texcoord_type), &_texcoords[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(_shader->getTexCoordLoc());
    glVertexAttribPointer(_shader->getTexCoordLoc(), 2, GL_FLOAT, 0, 0, 0);

    buffer_[2].bind();
    buffer_[2].data(_normals.size()*sizeof(normal_type), &_normals[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(_shader->getNormalLoc());
    glVertexAttribPointer(_shader->getNormalLoc(), 3, GL_FLOAT, 0, 0, 0);
}

void TriangleStrip::clear()
{
    _vertii.clear();
    _texcoords.clear();
    _normals.clear();
}

void TriangleStrip::setTexture(Texture::sptr texturein)
{
    _texture = texturein;
}

} // namespace client
} // namespace amethyst
