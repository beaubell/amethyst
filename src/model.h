#ifndef MODEL_H
#define MODEL_H

/***************************************************************************
 *   Model loading and processing function prototypes                      *
 *  (c) 2006 Beau V.C. Bellamy (beau@stellarnetservices.net)               *
 ***************************************************************************/

void load_models(void);
void free_models(void);

void renderSphere( float cx, float cy, float cz, float r, int p );

#endif /* MODEL_H */
