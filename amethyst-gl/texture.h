#ifndef TEXTURE_H
#define TEXTURE_H

/***************************************************************************
 *   Texture loading and processing function prototypes                    *
 *   (c) 2006 Beau V.C. Bellamy (beau@stellarnetservices.net)              *
 ***************************************************************************/

#include "opengl.h"

typedef struct {
    int width;
    int height;
	char *data;
} textureImage;

GLuint image_load  (const char *file);
void   load_skybox (void);
void   skybox (void);
//int    image_bmp_load(const char *filename, textureImage *texture);
bool   getBitmapImageData(const char *pFileName, textureImage *pImage );

#endif /* TEXTURE_H */
