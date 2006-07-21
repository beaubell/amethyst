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

void load_models(void);
void free_models(void);

Model* load_model_file(const std::string &filename);

void renderSphere( float cx, float cy, float cz, float r, int p );

#endif /* MODEL_H */
