#ifndef TEXTURE_H
#define TEXTURE_H

/***************************************************************************
 *   Texture loading and processing function prototypes                    *
 *   (c) 2006 Beau V.C. Bellamy (beau@stellarnetservices.net)              *
 ***************************************************************************/

#include "SDL_opengl.h"

typedef struct {
    int width;
    int height;
	char *data;
} textureImage;

GLuint load_image  (const char *file);
void   load_skybox (void);
void   skybox (void);
int    loadBMP(const char *filename, textureImage *texture);
bool   getBitmapImageData(const char *pFileName, textureImage *pImage );

#endif /* TEXTURE_H */
