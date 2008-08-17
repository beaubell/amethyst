#ifndef MODEL_H
#define MODEL_H

/***************************************************************************
 *   Model loading and processing function prototypes                      *
 *  (c) 2006 Beau V.C. Bellamy (beau@stellarnetservices.net)               *
 ***************************************************************************/

#include <string>

class Model
{
   public:
    unsigned int   format;
    unsigned int   vertices;
    float         *data;
};

void models_load(void);
void models_free(void);

Model* model_load_file(const std::string &filename);

void model_sphere_create( float cx, float cy, float cz, float r, int p );

#endif /* MODEL_H */
