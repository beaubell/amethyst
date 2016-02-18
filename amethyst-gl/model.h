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

#include <string>
#include <list>

#include "texture.h"

#include <boost/shared_ptr.hpp>

namespace amethyst {
namespace client {

class Model
{
   public:
    typedef boost::shared_ptr<Model> ptr;
    
    std::string    name;
    unsigned int   format;
    unsigned int   vertices;
    float         *data;
    GLuint         texture;
    GLuint         dl;

    Model();
    ~Model();

};

extern std::list<Model::ptr>  model_list;

void model_add(Model::ptr newmodel);
Model::ptr model_find(const std::string &name);
void models_free(void);

Model::ptr model_load(std::string &model_name);


void model_load_file(const std::string &filename, Model &model);

void model_sphere_create(double cx, double cy, double cz, double r, int p );

} // namespace client
} // namsspace amethyst

#endif /* AMETHYST_CLIENT_MODEL_H */
