#pragma once

/***********************************************************************
 Amethyst-GL
  - Texture loading and processing function prototypes

 Authors (c):
 2006      Jason A. Guild    (aguild@gmail.com)
 2006-2023 Beau V.C. Bellamy (bellamy.beau@gmail.com)
 ***********************************************************************/

#include "opengl.h"
#include "lib/resource.h"

#include <string>
#include <list>
#include <memory>

namespace amethyst::client {

typedef struct {
    GLsizei width;
    GLsizei height;
    std::unique_ptr<char *> data;
} TextureData;

class Texture
{
   public:
    typedef std::shared_ptr<Texture> sptr;
    typedef std::weak_ptr<Texture>   wptr;

    typedef unsigned int TextureHDL;
    typedef unsigned int TextureType;

    explicit Texture(const TextureType& type);
    virtual ~Texture();

    const std::string& getName();
    void setName(const std::string& newname);

    TextureHDL operator ()();
    
    virtual void bind();

    void image2D(GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid * data);

    void load(const lib::Resource& res);

  protected:
    // GLenum type
    TextureType type_;

    // Texture handle
    TextureHDL texture_;

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

bool   getBitmapImageData(const lib::Resource& res, TextureData *pImage );

} // namespace amethyst::client
