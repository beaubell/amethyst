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

namespace amethyst {
namespace client {

class Model
{
   public:
    std::string    name;
    unsigned int   format;
    unsigned int   vertices;
    float         *data;
    GLuint         texture;
    GLuint         dl;

};

extern std::list<Model *>  model_list;

void model_add(Model * newmodel);
Model* model_find(const std::string &name);
void models_free(void);

Model* model_load(std::string &model_name);


void model_load_file(const std::string &filename, Model &model);

void model_sphere_create(double cx, double cy, double cz, double r, int p );

} // namespace client
} // namsspace amethyst

#endif /* AMETHYST_CLIENT_MODEL_H */
