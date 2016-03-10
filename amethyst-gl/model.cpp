/***********************************************************************
 Amethyst-GL
  - Model loading and processing function implementations

 Authors (c):
 2004-2008 Beau V.C. Bellamy (beau@stellarnetservices.net)

 $Revision$
 $LastChangedDate$
 $LastChangedBy$
 ***********************************************************************/

#include "model.h"
#include "model_xml.h"

#include "file.h"

#include "global.h"
#include "texture.h"

#include <math.h>    // cosf, sinf
#include <string>    // std::string

#include <stdlib.h>  // malloc
#include <errno.h>   // errno
#include <iostream>
#include <stdexcept>
#include <functional>

using namespace std::placeholders;

#define SPHERE_DETAIL 100

namespace amethyst {
namespace client {

std::list<Model::sptr>  model_list;

//
//
Primative::Primative(const std::string& name)
 : _name(name)
{}


Primative::~Primative()
{}

void Primative::setName(const std::string& name)
{
    _name = name;
}

const std::string& Primative::getName()
{
    return _name;
}


//
//
TriangleStrip::TriangleStrip()
 : Primative("TriangleStrip"),
   _vertcount(0)
{}

TriangleStrip::TriangleStrip(const std::string& name, Texture::sptr texturein)
 : Primative(name),
   _texture(texturein),
   _vertcount(0)
{
}

TriangleStrip::~TriangleStrip()
{
}

void TriangleStrip::render(const TransMatrix& m_proj, const TransMatrix& m_view, const TransMatrix& m_model)
{
    if(!_shader)
        throw std::runtime_error("TriangleStrip not bound to shader " + this->getName());

    _shader->use();
    _shader->UniformMatrix4f(_mprojloc, m_proj);
    _shader->UniformMatrix4f(_mviewloc, m_view);
    _shader->UniformMatrix4f(_mmodeloc, m_model);
    _texture->bind();

    // Render vao objects
    glBindVertexArray(_vao[0]);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, _vertii.size());
}

void TriangleStrip::bind(ShaderProgram::sptr shaderprog)
{
    _shader  = shaderprog;
    _vertexloc = _shader->GetAttribLocation("vertexPosition");
    _texcoordloc = _shader->GetAttribLocation("texcoordData");
    //_normalloc   = _shader->GetAttribLocation("vertexNormal");
    _texunitloc  = _shader->GetAttribLocation("baseTex");

    glGenVertexArrays(1, _vao);
    glBindVertexArray(_vao[0]);

    // Generate buffer for frame;
    glGenBuffers(3, _buffer);

    glBindBuffer(GL_ARRAY_BUFFER, _buffer[0]);
    glBufferData(GL_ARRAY_BUFFER, _vertii.size()*sizeof(vertex_type), &_vertii[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(_vertexloc.value);
    glVertexAttribPointer(_vertexloc.value, 3, GL_FLOAT, 0, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, _buffer[1]);
    glBufferData(GL_ARRAY_BUFFER, _texcoords.size()*sizeof(texcoord_type), &_texcoords[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(_texcoordloc.value);
    glVertexAttribPointer(_texcoordloc.value, 2, GL_FLOAT, 0, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, _buffer[2]);
    glBufferData(GL_ARRAY_BUFFER, _normals.size()*sizeof(normal_type), &_normals[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(_normalloc.value);
    glVertexAttribPointer(_normalloc.value, 3, GL_FLOAT, 0, 0, 0);
}

void TriangleStrip::clear()
{
    glDeleteVertexArrays(1, _vao);

    _vertii.clear();
    _texcoords.clear();
    _normals.clear();

    _vertcount = 0;
}

void TriangleStrip::setTexture(Texture::sptr texturein)
{
    _texture = texturein;
}

void TriangleStrip::addVertex(const vertex_type &vertex, const texcoord_type &texcoord, const normal_type normal)
{
    _vertii.push_back(vertex);
    _texcoords.push_back(texcoord);
    _normals.push_back(normal);

    _vertcount++;
}

//
//
Model::Model()
{}

Model::~Model()
{

}

void Model::render(const TransMatrix& m_proj, const TransMatrix& m_view, const TransMatrix& m_model)
{
    std::for_each(_primatives.begin(), _primatives.end(),
       std::bind(&Primative::render, _1, m_proj, m_view, m_model));
}

void Model::addPrimative(Primative::sptr newprim)
{
    _primatives.push_back(newprim);
}

Model::sptr model_load(std::string &model_name)
{

    Model::sptr model = NULL;

    // See if model is already loaded
    model = model_find(model_name);

    // If model doesn't already exist, load and add to linked list
    if(!model)
    {
        model = std::make_shared<Model>();

        try
        {
            model_xml_load(model_name, *model);
        }
        catch (std::runtime_error &e)
        {
            throw e;
        }
        model->setName(model_name);
        model_add(model);
    }

    return model;
}


void model_add(Model::sptr newmodel)
{
    if (newmodel)
      model_list.push_back(newmodel);

    return;
}


Model::sptr model_find(const std::string &name)
{
    if(!model_list.empty())
    {
        auto obj1 = model_list.begin();
        do
        {
            if(name == (*obj1)->getName())
                return *obj1;

            obj1++;
        }  while (obj1 != model_list.end());
    }
    return NULL;
}


void models_free(void)
{
    model_list.clear();

    return;
}


TriangleStrip::sptr model_sphere_create(const double cx, const double cy, const double cz, double r, int p )
{
    // Make new Triangle Strip object
    TriangleStrip::sptr sphere = std::make_shared<TriangleStrip>();
    sphere->setName("Sphere");

    TriangleStrip::vertex_type point;
    TriangleStrip::texcoord_type tex;
    TriangleStrip::normal_type normal;


    const double TWOPI  = 6.28318530717958647692;
    const double PIDIV2 = 1.57079632679489661923;

    double theta1 = 0.0;
    double theta2 = 0.0;
    double theta3 = 0.0;

    // Disallow a negative number for radius.
    if( r < 0 )
        r = -r;

	// Disallow a negative number for precision.
    if( p < 0 )
        p = -p;

    for( int i = 0; i < p/2; ++i )
    {
        theta1 = i * TWOPI / p - PIDIV2;
        theta2 = (i + 1) * TWOPI / p - PIDIV2;

        for( int j = 0; j <= p; j++ )
        {
            theta3 = j * TWOPI / p;

            normal.x = cos(theta2) * cos(theta3);
            normal.z = sin(theta2);
            normal.y = cos(theta2) * sin(theta3);
            point.x = cx + r * normal.x;
            point.z = cz + r * normal.z;
            point.y = cy + r * normal.y;
            tex.s = j/static_cast<double>(p);
            tex.t = 2.0*(i+1)/static_cast<double>(p);

            sphere->addVertex(point, tex, normal);

            normal.x = cos(theta1) * cos(theta3);
            normal.z = sin(theta1);
            normal.y = cos(theta1) * sin(theta3);
            point.x = cx + r * normal.x;
            point.z = cz + r * normal.z;
            point.y = cy + r * normal.y;
            tex.s = j/static_cast<double>(p);
            tex.t = 2.0*i/static_cast<double>(p);

            sphere->addVertex(point, tex, normal);
        }
    }

    return sphere;
}


void model_load_file(const std::string &filename, Model &model, Texture::sptr tex, ShaderProgram::sptr shdr)
{
    if (access(filename.c_str(), R_OK) < 0) {
        if (errno == ENOENT)
            printf ("Model file doesn't exist: %s\n", filename.c_str());
        if (errno == EACCES)
            printf ("Access denied accessing model file: %s\n", filename.c_str());
	throw std::runtime_error("Cannot Access Model File");
    }

    unsigned int vertices = 0, vertices_t = 0, i = 0;
    FILE *file = fopen(filename.c_str(), "r");

    if(!file)
        throw(std::runtime_error("Failed to open model file"));

    //Create New Primative
    TriangleStrip::sptr prim = std::make_shared<TriangleStrip>();

    TriangleStrip::vertex_type point;
    TriangleStrip::texcoord_type texcoord;
    TriangleStrip::normal_type normal;

    prim->setName("Main");

    fscanf(file, "%d\n", &vertices);

    vertices_t = vertices * 8;

    for (i = 0; !feof(file) && i < vertices_t; i += 8)
    {
        fscanf(file, "%f, %f,",      &(texcoord.s), &(texcoord.t));
        fscanf(file, "%f, %f, %f,",  &(normal.x), &(normal.y), &(normal.z));
        fscanf(file, "%f, %f, %f\n", &(point.x), &(point.y), &(point.z));

        prim->addVertex(point, texcoord, normal);
    }

    if (i != vertices_t) {
        printf("ERROR: Model file appears corrupted: Got (%i), expected (%i)\n", i, vertices_t);
        vertices = i;
    }

    prim->setTexture(tex);
    prim->bind(shdr);
    model.addPrimative(prim);

    fclose(file);
}

} // namespace client
} // namespace amethyst
