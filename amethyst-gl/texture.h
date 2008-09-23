#ifndef TEXTURE_H
#define TEXTURE_H

/***************************************************************************
 *   Texture loading and processing function prototypes                    *
 *   (c) 2006 Beau V.C. Bellamy (beau@stellarnetservices.net)              *
 ***************************************************************************/

#include "opengl.h"
#include <string>
#include <list>

typedef struct {
    int width;
    int height;
	char *data;
} textureImage;

class Texture
{
   public:
    std::string    name;
    unsigned int   width;
    unsigned int   height;
    GLuint         gl_id;  // OpenGL Texture index

};

extern std::list<Texture *>  texture_list;

GLuint texture_load(std::string &texture_name);

GLuint image_load  (const char *file);
void   load_skybox (void);
void   skybox (void);
//int    image_bmp_load(const char *filename, textureImage *texture);
bool   getBitmapImageData(const char *pFileName, textureImage *pImage );

#endif /* TEXTURE_H */
