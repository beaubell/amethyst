/***********************************************************************
 Amethyst-GL
  - Texture loading and processing function implimentations

 Authors (c):
 2006      Jason A. Guild    (aguild@gmail.com)
 2006-2008 Beau V.C. Bellamy (beau@stellarnetservices.net)

 $Revision$
 $LastChangedDate$
 $LastChangedBy$
 ***********************************************************************/

#include "file.h"

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

Texture2D::sptr TexSkyBox[6];

#define BACK_ID 0
#define FRONT_ID 1
#define BOTTOM_ID 2
#define TOP_ID 3
#define RIGHT_ID 4
#define LEFT_ID 5


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


void Texture::setName(const std::__cxx11::string& newname)
{
    name_ = newname;
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
    
}


//
//
Texture2D::Texture2D()
 : Texture(GL_TEXTURE_2D)
{
}



//
//
void Texture::load(const std::string& filename)
{
    textureImage *texti;
    uint num_mipmaps = 2;

    if (access(filename.c_str(), R_OK) < 0) {
       if (errno == ENOENT)
          printf ("Texture file doesn't exist: %s\n", filename.c_str()); //FIXME
       if (errno == EACCES)
          printf ("Access denied accessing texture file: %s\n", filename.c_str()); //FIXME
       return;
    }

    texti = new textureImage;
    getBitmapImageData(filename.c_str(), texti);

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
            std::string texture_path = Global.dir_textures + texture_name;
            texture->load(texture_path);
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


void load_skybox (void)
{

    std::string tex_sb_bk     = Global.dir_textures + "/" + Global.file_tex_sb_bk;
    std::string tex_sb_ft     = Global.dir_textures + "/" + Global.file_tex_sb_ft;
    std::string tex_sb_up     = Global.dir_textures + "/" + Global.file_tex_sb_up;
    std::string tex_sb_dn     = Global.dir_textures + "/" + Global.file_tex_sb_dn;
    std::string tex_sb_rt     = Global.dir_textures + "/" + Global.file_tex_sb_rt;
    std::string tex_sb_lt     = Global.dir_textures + "/" + Global.file_tex_sb_lt;

    for (uint i = 0; i < 6; i++)
        TexSkyBox[i] = std::make_shared<Texture2D>();

    TexSkyBox[BACK_ID]->load(tex_sb_bk);
    TexSkyBox[FRONT_ID]->load(tex_sb_ft);
    TexSkyBox[BOTTOM_ID]->load(tex_sb_dn);
    TexSkyBox[TOP_ID]->load(tex_sb_up);
    TexSkyBox[RIGHT_ID]->load(tex_sb_rt);
    TexSkyBox[LEFT_ID]->load(tex_sb_lt);

    for(uint i = 0; i < 6; ++i)
    {
        TexSkyBox[i]->bind();
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, 0x812F);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, 0x812F);

        //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    }
}

void skybox (void)
{
  float x = 0;
  float y = 0;
  float z = 0;
  float width = 50000;
  float height = 50000;
  float length = 50000;

  x = x - width  / 2;  // Calcul l'emplacement d'un coin du cube
  y = y - height / 2;
  z = z - length / 2;

  glDisable(GL_DEPTH_TEST);
  glDepthMask(GL_FALSE);

  TexSkyBox[BACK_ID]->bind();
  glBegin(GL_TRIANGLE_STRIP);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(x,y,z);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y,z);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(x,y + height, z);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height, z);
  glEnd();

  TexSkyBox[FRONT_ID]->bind();
  glBegin(GL_TRIANGLE_STRIP);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(x,y,z + length);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(x,y + height, z + length);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y, z + length);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z + length);
  glEnd();

  TexSkyBox[BOTTOM_ID]->bind();
  glBegin(GL_TRIANGLE_STRIP);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(x,y,z);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(x,y,       z + length);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y,z);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y,z + length);
  glEnd();

  TexSkyBox[TOP_ID]->bind();
  glBegin(GL_TRIANGLE_STRIP);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x,y + height,z);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y + height, z + length);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y + height, z + length);
  glEnd();


  TexSkyBox[RIGHT_ID]->bind();
  glBegin(GL_TRIANGLE_STRIP);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y, z);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + height, z);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y, z + length);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y + height, z + length);
  glEnd();

  TexSkyBox[LEFT_ID]->bind();
  glBegin(GL_TRIANGLE_STRIP);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y, z);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y, z + length);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height, z + length);
  glEnd();

  glDepthMask(GL_TRUE);
  glEnable(GL_DEPTH_TEST);
}


//-----------------------------------------------------------------------------
// Name: getBitmapImageData()
// Desc: Simply image loader for 24 bit BMP files.
// Shamelessly Stolen From: Nehe
//-----------------------------------------------------------------------------
bool getBitmapImageData(const char *pFileName, textureImage *pImage )
{
    SDL_RWops *pFile = NULL;
    unsigned short nNumPlanes;
    unsigned short nNumBPP;
    Uint32 i;
    Uint32 t = 0;
    Uint16 t2 = 0;

	if( (pFile = SDL_RWFromFile(pFileName, "rb") ) == NULL)
       std::cout << "ERROR: getBitmapImageData - %s not found " << pFileName << "." << std::endl;

    // Seek forward to width and height info
	SDL_RWseek(pFile, 18, SEEK_CUR);

	if ( ( i = SDL_RWread(pFile, &t, 4, 1) ) != 1)
       std::cout << "ERROR: getBitmapImageData - Couldn't read width from " << pFileName << "." << std::endl;
	pImage->width = SDL_SwapLE32(t);

	if ( ( i = SDL_RWread(pFile, &t, 4, 1) ) != 1)
       std::cout << "ERROR: getBitmapImageData - Couldn't read height from " << pFileName << "." << std::endl;
	pImage->height = SDL_SwapLE32(t);

	if ( ( i = SDL_RWread(pFile, &t2, 2, 1) ) != 1)
       std::cout << "ERROR: getBitmapImageData - Couldn't read plane count from " << pFileName << "." << std::endl;
    nNumPlanes = SDL_SwapLE16(t2);

    if( nNumPlanes != 1 )
       std::cout << "ERROR: getBitmapImageData - Plane count from " << pFileName
            << " is not 1: " << nNumPlanes << "." << std::endl;

	if ( ( i = SDL_RWread(pFile, &t2, 2, 1) ) != 1)
       std::cout << "ERROR: getBitmapImageData - Couldn't read BPP from " << pFileName << std::endl;
    nNumBPP = SDL_SwapLE16(t2);

    if( nNumBPP != 24 )
       std::cout << "ERROR: getBitmapImageData - BPP from " << pFileName
            << " is not 24: " << nNumBPP << "." << std::endl;

    // Seek forward to image data
    SDL_RWseek( pFile, 24, SEEK_CUR );

    // Calculate the image's total size in bytes. Note how we multiply the
    // result of (width * height) by 3. This is becuase a 24 bit color BMP
    // file will give you 3 bytes per pixel.
    unsigned int nTotalImagesize = (pImage->width * pImage->height) * 3;

    pImage->data = new char[nTotalImagesize];

    if( (i = SDL_RWread(pFile, pImage->data, 1, nTotalImagesize) ) != nTotalImagesize )
       std::cout << "ERROR: getBitmapImageData - Couldn't read image data from " << pFileName << "." << std::endl;

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

/*
    char charTemp;
    for( i = 0; i < nTotalImagesize; i += 3 ) {
        charTemp = pImage->data[i];
        pImage->data[i] = pImage->data[i+2];
        pImage->data[i+2] = charTemp;
    }
*/
    printf("Loaded BMP %dbit (%dx%d): %s (%d)\n", nNumBPP, pImage->width, pImage->height, pFileName, nTotalImagesize);

    SDL_RWclose(pFile);

    return true;
}

} // namespace client
} // namespace amethyst
