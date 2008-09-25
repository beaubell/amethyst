#ifndef MODEL_H
#define MODEL_H

/***********************************************************************
 Amethyst-GL
  - Model loading and processing function prototypes

 Authors (c):
 2004-2008 Beau V.C. Bellamy (beau@stellarnetservices.net)

 $Revision: 150 $
 $LastChangedDate: 2008-09-23 13:43:06 -0800 (Tue, 23 Sep 2008) $
 $LastChangedBy: beau $
 ***********************************************************************/

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
