#ifndef AMETHYST_CLIENT_TEXTURE_H
#define AMETHYST_CLIENT_TEXTURE_H

/***********************************************************************
 Amethyst-GL
  - Texture loading and processing function prototypes

 Authors (c):
 2006      Jason A. Guild    (aguild@gmail.com)
 2006-2008 Beau V.C. Bellamy (beau@stellarnetservices.net)

 $Revision$
 $LastChangedDate$
 $LastChangedBy$
 ***********************************************************************/

#include "opengl.h"
#include <string>
#include <list>

namespace amethyst {
namespace client {

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

GLuint texture_load(const std::string &texture_name);
void texture_add(Texture *newtexture);
Texture* texture_find(const std::string &name);
void textures_free(void);

GLuint image_load  (const char *file);
void   load_skybox (void);
void   skybox (void);
//int    image_bmp_load(const char *filename, textureImage *texture);
bool   getBitmapImageData(const char *pFileName, textureImage *pImage );

} // namespace client
} // namespace amethyst

#endif /* AMETHYST_CLIENT_TEXTURE_H */
