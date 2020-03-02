/***********************************************************************
 Amethyst-GL
 - Model manager and factory

 Authors (c):
 2020 Beau V.C. Bellamy (bellamy.beau@gmail.com)
 ***********************************************************************/

#include "model_manager.h"

#include "global.h"
#include "yaml-cpp/yaml.h"

#include <iostream>

namespace amethyst::client {

ModelManager modelmanager;


#define SPHERE_DETAIL 100

void model_yaml_load(std::string &name, Model &model);
void model_load_file(const Resource& res, Model &model, Texture::sptr tex, ShaderProgramModel::sptr shdr);
TriangleStrip::sptr model_sphere_create(double cx, double cy, double cz, double r, int p );



ModelManager::ModelManager()
{
    // Nothing to do
}

ModelManager::~ModelManager()
{
    clear();
}


Model::sptr
ModelManager::get(std::string &model_name)
{

    Model::sptr model = NULL;

    // See if model is already loaded
    model = find(model_name);

    // If model doesn't already exist, load and add to linked list
    if (!model) {
        model = std::make_shared<Model>();

        try {
            model_yaml_load(model_name, *model);
        } catch (std::runtime_error &e) {
            std::cout << "Error loading model \"" << model_name << "\":" << e.what() << std::endl;

            throw;
        }
        model->setName(model_name);
        add(model);
    }

    return model;
}


void
ModelManager::add(Model::sptr newmodel)
{
    if (newmodel)
        models_[newmodel->getName()] = newmodel;

    return;
}


Model::sptr
ModelManager::find(const std::string &name)
{
    if (models_.count(name) == 0)
        return nullptr;

    return models_[name];
}


void
ModelManager::clear(void)
{
    models_.clear();

    return;
}


TriangleStrip::sptr
model_sphere_create(const double cx, const double cy, const double cz, double r, int p)
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
    if (r < 0)
        r = -r;

    // Disallow a negative number for precision.
    if (p < 0)
        p = -p;

    for (int i = 0; i < p / 2; ++i) {
        theta1 = i * TWOPI / p - PIDIV2;
        theta2 = (i + 1) * TWOPI / p - PIDIV2;

        for (int j = 0; j <= p; j++) {
            theta3 = j * TWOPI / p;

            normal.x = cos(theta2) * cos(theta3);
            normal.z = sin(theta2);
            normal.y = cos(theta2) * sin(theta3);
            point.x = cx + r * normal.x;
            point.z = cz + r * normal.z;
            point.y = cy + r * normal.y;
            tex.s = j / static_cast<double>(p);
            tex.t = 2.0 * (i + 1) / static_cast<double>(p);

            sphere->addVertex(point, tex, normal);

            normal.x = cos(theta1) * cos(theta3);
            normal.z = sin(theta1);
            normal.y = cos(theta1) * sin(theta3);
            point.x = cx + r * normal.x;
            point.z = cz + r * normal.z;
            point.y = cy + r * normal.y;
            tex.s = j / static_cast<double>(p);
            tex.t = 2.0 * i / static_cast<double>(p);

            sphere->addVertex(point, tex, normal);
        }
    }

    return sphere;
}


void model_yaml_load(std::string &name, Model &model) {

    using namespace YAML;

    std::string filename = Global.dir_models + "mdl_" + name + ".yaml";

    Node yroot = YAML::LoadFile(filename);
    Node ymodel = yroot["model"];
    if (!ymodel.IsMap()) {
        // ERROR
        // TODO
        return;
    }

    if (ymodel["name"].IsScalar())
        model.setName(ymodel["name"].as<std::string>());

    for (auto yc: ymodel["components"]) {
        if (yc["sphere"].IsDefined()) {
            Node ysphere = yc["sphere"];

            float_type radius = ysphere["radius"].as<float_type>();
            int precision     = ysphere["precision"].as<int>();
            std::string texture_name = ysphere["texture"].as<std::string>();

            // Get Material Info
            // TODO

            // Make Model and Load Resources
            Texture::sptr tex = texture_load(texture_name);
            ShaderProgramModel::sptr shdr = std::make_shared<ShaderProgramModel>();


            TriangleStrip::sptr sphere = model_sphere_create(0.0, 0.0, 0.0, radius, precision);
            sphere->setTexture(tex);
            sphere->bind(shdr);

            model.addPrimitive(sphere);

        }

        if (yc["extfile"].IsDefined()) {
            Node yextfile = yc["extfile"];

            std::string mdlfile = yextfile.as<std::string>();
            Texture::sptr tex;
            ShaderProgramModel::sptr shdr = std::make_shared<ShaderProgramModel>();

            model_load_file(Resource(Global.dir_models, mdlfile), model, tex, shdr);

        }
    }

}


void
model_load_file(const Resource &res, Model &model, Texture::sptr tex, ShaderProgramModel::sptr shdr)
{
    std::cout << "Loading Model File: " << res.name() << std::endl;

    unsigned int vertices = 0, vertices_t = 0, i = 0;

    //Create New Primative
    Triangles::sptr prim = std::make_shared<Triangles>();

    Triangles::vertex_type point;
    Triangles::texcoord_type texcoord;
    Triangles::normal_type normal;

    prim->setName("Main");

    auto &s = res.get_stream();

    std::string line;
    std::getline(s, line);

    sscanf(line.c_str(), "%d\n", &vertices);

    vertices_t = vertices * 8;

    for (i = 0; std::getline(s, line) && i < vertices_t; i += 8) {
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

} // namespace amethyst::client
