/***********************************************************************
 Amethyst-GL
  - Model loading and processing function implementations

 Authors (c):
 2004-2020 Beau V.C. Bellamy (bellamy.beau@gmail.com)
 ***********************************************************************/

#include "model.h"
#include "model_xml.h"

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
Model::Model()
{}

Model::~Model()
{

}

void Model::render(const TransMatrix& m_proj, const TransMatrix& m_view, const TransMatrix& m_model)
{
    std::for_each(_primitives.begin(), _primitives.end(),
       std::bind(&Primitive::render, _1, m_proj, m_view, m_model));
}

void Model::addPrimitive(Primitive::sptr newprim)
{
    _primitives.push_back(newprim);
}

const std::list<Primitive::sptr>& Model::getPrimitives()
{
    return _primitives;
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


void model_load_file(const Resource &res, Model &model, Texture::sptr tex, ShaderProgramModel::sptr shdr)
{
    std::cout << "Loading Model File: " << res.name() << std::endl;
 
    unsigned int vertices = 0, vertices_t = 0, i = 0;

    //Create New Primative
    Triangles::sptr prim = std::make_shared<Triangles>();

    Triangles::vertex_type point;
    Triangles::texcoord_type texcoord;
    Triangles::normal_type normal;

    prim->setName("Main");

    auto& s = res.get_stream();

    std::string line;
    std::getline(s, line);

    sscanf(line.c_str(), "%d\n", &vertices);
    
    vertices_t = vertices * 8;

    for (i = 0; std::getline(s, line) && i < vertices_t; i += 8)
    {
        sscanf(line.c_str(), "%f, %f, %f, %f, %f, %f, %f, %f\n", 
               &(texcoord.s), &(texcoord.t),
               &(normal.x), &(normal.y), &(normal.z),
               &(point.x), &(point.y), &(point.z));

        prim->addVertex(point, texcoord, normal);
    }

    if (i != vertices_t) {
        printf("ERROR: Model file appears corrupted: Got (%i), expected (%i)\n", i, vertices_t);
        vertices = i;
    }

    prim->setTexture(tex);
    prim->bind(shdr);
    model.addPrimitive(prim);
}

} // namespace client
} // namespace amethyst
