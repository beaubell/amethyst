#ifndef MODEL_H
#define MODEL_H

/***************************************************************************
 *   Model loading and processing function prototypes                      *
 *  (c) 2006 Beau V.C. Bellamy (beau@stellarnetservices.net)               *
 ***************************************************************************/

#include <string>
#include <list>

#include "texture.h"

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

void models_load(void);
void models_free(void);

Model* model_load(std::string &model);

Model* model_load_file(const std::string &filename);

void model_sphere_create(double cx, double cy, double cz, double r, int p );

#endif /* MODEL_H */
