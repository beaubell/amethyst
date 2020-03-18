/***********************************************************************
 Amethyst-GL
  - Texture loading and processing function implimentations

 Authors (c):
 2006      Jason A. Guild    (aguild@gmail.com)
 2006-2020 Beau V.C. Bellamy (beau@bellamy.beau@gmail.com)
 ***********************************************************************/

#include "texture.h"

#include "lib/object.h"
#include <iostream>
#include <stdexcept>
#include <stdio.h>
#include <errno.h>

#include "global.h"

namespace amethyst {
namespace client {

extern __global Global;


std::list<Texture::wptr>  texture_list;

Texture::Texture(const TextureType& type)
    : type_(type),
      texture_(0),
      name_("null")
{
    glGenTextures(1, &texture_);
}


Texture::~Texture()
{
    glDeleteTextures(1, &texture_);
}


const std::string& Texture::getName()
{
    return name_;
}


void Texture::setName(const std::string& newname)
{
    name_ = newname;
}


Texture::TextureHDL Texture::operator ()()
{
    return texture_;
}

void Texture::bind()
{
    glBindTexture(type_, texture_);
    glTexParameteri(type_, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(type_, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
}


void Texture::image2D(GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid* data)
{
    this->bind();
    glTexImage2D(type_, level, internalFormat, width, height, border,format, type, data);
}


//
//
Texture2D::Texture2D()
    : Texture(GL_TEXTURE_2D)
{
}



//
//
void Texture::load(const Resource& res)
{
    textureImage *texti;
    unsigned int  num_mipmaps = 2; //TODO, don't hardcode this.

    texti = new textureImage;
    getBitmapImageData(res, texti);

    // set unpacking method
    glPixelStorei (GL_UNPACK_ALIGNMENT, 1);

    // Tell opengl we want to start playing with texture
    this->bind();

    // Create texture storage space
    glTexStorage2D(GL_TEXTURE_2D, num_mipmaps, GL_RGB8, texti->width, texti->height);

    // Load
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, texti->width, texti->height, GL_RGB, GL_UNSIGNED_BYTE, texti->data);

    // Generate mipmap levels
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    delete [] texti->data;
    delete texti;
}


Texture::sptr texture_load(const std::string &texture_name)
{

    Texture::sptr texture;

    texture = texture_find(texture_name);

    if(!texture)
    {
        try
        {
            texture = std::make_shared<Texture2D>();
            Resource res(Global.dir_textures, texture_name);
            texture->load(res);
        }
        catch(std::runtime_error &e)
        {
            texture = NULL;
            throw e;
        }
        texture->setName(texture_name);
        texture_add(texture);
    }

    return texture;
}


void texture_add(Texture::sptr newtexture)
{
    if (newtexture)
        texture_list.push_back(newtexture);

    return;
}


Texture::sptr texture_find(const std::string &name)
{
    if(!texture_list.empty())
    {
        auto texwptr_it = texture_list.begin();
        do
        {
            if(auto texsptr = texwptr_it->lock())
            {
                if(name == texsptr->getName())
                    return texsptr;

                texwptr_it++;
            }
            else // Purge hanging weak_ptr
            {
                texwptr_it = texture_list.erase(texwptr_it);
            }

        }  while (texwptr_it != texture_list.end());
    }
    return NULL;
}


void textures_free(void)
{
    texture_list.clear();
}



//-----------------------------------------------------------------------------
// Name: getBitmapImageData()
// Desc: Simply image loader for 24 bit BMP files.
// Shamelessly Stolen From: Nehe
//-----------------------------------------------------------------------------
bool getBitmapImageData(const Resource& res, textureImage *pImage )
{
    const size_t BMP_WIDTH_OFF = 18;
    const size_t BMP_HEIGHT_OFF = 22;
    const size_t BMP_NPLANES_OFF = 26;
    const size_t BMP_NBPP_OFF = 28;
    const size_t BMP_DATA_OFF = 54;
    
    unsigned short nNumPlanes;
    unsigned short nNumBPP;

    Uint32 t = 0;
    Uint16 t2 = 0;

    // Parse Header
    pImage->width = SDL_SwapLE32(res.getUInt32(BMP_WIDTH_OFF));
    pImage->height = SDL_SwapLE32(res.getUInt32(BMP_HEIGHT_OFF));
    nNumPlanes = SDL_SwapLE16(res.getUInt16(BMP_NPLANES_OFF));

    if( nNumPlanes != 1 )
        std::cout << "ERROR: getBitmapImageData - Plane count from " << res.name()
                  << " is not 1: " << nNumPlanes << "." << std::endl;

    nNumBPP = SDL_SwapLE16(res.getUInt16(BMP_NBPP_OFF));

    if( nNumBPP != 24 )
        std::cout << "ERROR: getBitmapImageData - BPP from " << res.name()
                  << " is not 24: " << nNumBPP << "." << std::endl;


    // Calculate the image's total size in bytes. Note how we multiply the
    // result of (width * height) by 3. This is becuase a 24 bit color BMP
    // file will give you 3 bytes per pixel.
    unsigned int nTotalImagesize = (pImage->width * pImage->height) * 3;

    pImage->data = new char[nTotalImagesize];
    

    for (size_t i = 0; i < nTotalImagesize; i++) {
        pImage->data[i] = res[BMP_DATA_OFF + i];
    }

    //
    // Finally, rearrange BGR to RGB
    //

    int length = (pImage->width * 3 + 3) & ~3;
    //printf("length: %d\n", length);
    //printf("image size: %d\n", nTotalImagesize);
    int x, y;
    char *ptr, temp;
    for (y = 0; y < pImage->height; y ++)
        for (ptr = pImage->data + y * length, x = pImage->width;
                x > 0;
                x--, ptr += 3)
        {
            temp   = ptr[0];
            ptr[0] = ptr[2];
            ptr[2] = temp;
        }

    printf("Loaded BMP %dbit (%dx%d): %s (%d)\n", nNumBPP, pImage->width, pImage->height, res.name().c_str(), nTotalImagesize);

    return true;
}


} // namespace client
} // namespace amethyst
