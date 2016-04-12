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
#include <memory>

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
    typedef std::shared_ptr<Texture> sptr;
    typedef std::weak_ptr<Texture>   wptr;

    typedef unsigned int TextureHDL;
    typedef unsigned int TextureType;

    Texture(const TextureType& type);
    virtual ~Texture();

    const std::string& getName();
    void setName(const std::string& newname);

    virtual void bind();

    void image2D(GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid * data);

    void load(const std::string& filename);

  protected:
    // Texture handle
    TextureHDL texture_;

    // GLenum type
    TextureType type_;

  private:
    std::string    name_;
};



class Texture2D : public Texture
{
   public:
    typedef std::shared_ptr<Texture2D> sptr;
    typedef std::weak_ptr<Texture2D>   wptr;

    Texture2D();
};


extern std::list<Texture::wptr>  texture_list;

Texture::sptr texture_load(const std::string &texture_name);
void texture_add(Texture::sptr newtexture);
Texture::sptr texture_find(const std::string &name);
void textures_free(void);

void   load_skybox (void);
void   skybox (void);

bool   getBitmapImageData(const char *pFileName, textureImage *pImage );

} // namespace client
} // namespace amethyst

#endif /* AMETHYST_CLIENT_TEXTURE_H */
