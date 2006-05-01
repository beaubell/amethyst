/***************************************************************************
 *   Texture loading and processing function implimentation                *
 *   (c) 2006 Beau V.C. Bellamy (beau@stellarnetservices.net)              *
 ***************************************************************************/



#include "SDL_opengl.h"


#include <amethyst/object.h>
#include <iostream>

#include <stdio.h>
#include <errno.h>

#include "global.h"
#include "texture.h"

using namespace std;
using namespace amethyst;

extern __global Global;
GLuint TexIDSkyBox[10];

#define BACK_ID 0
#define FRONT_ID 1
#define BOTTOM_ID 2
#define TOP_ID 3
#define RIGHT_ID 4
#define LEFT_ID 5

// FIXME figure out how to pass errors downstream
GLuint load_image(const char *file) {

    GLuint texture;
    textureImage *texti;

    if (access(file, R_OK) < 0) {
       if (errno == ENOENT)
          printf ("Texture file doesn't exist: %s\n", file);
       if (errno == EACCES)
          printf ("Access denied accessing texture file: %s\n", file);
       return 0;
    }

    texti = (textureImage *)malloc(sizeof(textureImage));
    getBitmapImageData(file, texti);

    // allocate a texture segment and assign value to 'texture'
    glGenTextures(1, &texture);

    // set unpacking method
    glPixelStorei (GL_UNPACK_ALIGNMENT, 1);

    // Tell opengl we want to start playing with texture number assigned to 'texture'
    glBindTexture(GL_TEXTURE_2D, texture);

    // Load 
    //gluBuild2DMipmaps(GL_TEXTURE_2D, 3, image->w,image->h,GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
    //glTexImage2D(GL_TEXTURE_2D, 0, 3, test->w, test->h, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, texti->width, texti->height, 0, GL_RGB, GL_UNSIGNED_BYTE, texti->data);

    free (texti->data);
    free (texti);
    return texture;

    }

void load_skybox (void)
{
    int i=1;

    string tex_sb_bk     = Global.dir_textures + "/" + Global.file_tex_sb_bk;
    string tex_sb_ft     = Global.dir_textures + "/" + Global.file_tex_sb_ft;
    string tex_sb_up     = Global.dir_textures + "/" + Global.file_tex_sb_up;
    string tex_sb_dn     = Global.dir_textures + "/" + Global.file_tex_sb_dn;
    string tex_sb_rt     = Global.dir_textures + "/" + Global.file_tex_sb_rt;
    string tex_sb_lt     = Global.dir_textures + "/" + Global.file_tex_sb_lt;

    TexIDSkyBox[0] = load_image(tex_sb_bk.c_str());
    TexIDSkyBox[1] = load_image(tex_sb_ft.c_str());
    TexIDSkyBox[2] = load_image(tex_sb_dn.c_str());
    TexIDSkyBox[3] = load_image(tex_sb_up.c_str());
    TexIDSkyBox[4] = load_image(tex_sb_rt.c_str());
    TexIDSkyBox[5] = load_image(tex_sb_lt.c_str());



    for(i=0;i<=5;++i)
    {
        glBindTexture(GL_TEXTURE_2D, TexIDSkyBox[i]);
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

  glBindTexture(GL_TEXTURE_2D, TexIDSkyBox[BACK_ID]);
  glBegin(GL_TRIANGLE_STRIP);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(x,y,z);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y,z);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(x,y + height, z);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height, z); 
  glEnd();

  glBindTexture(GL_TEXTURE_2D, TexIDSkyBox[FRONT_ID]);
  glBegin(GL_TRIANGLE_STRIP);                   
        glTexCoord2f(1.0f, 0.0f); glVertex3f(x,y,z + length);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(x,y + height, z + length);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y, z + length);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z + length);        
  glEnd();

  glBindTexture(GL_TEXTURE_2D, TexIDSkyBox[BOTTOM_ID]);
  glBegin(GL_TRIANGLE_STRIP);                           
        glTexCoord2f(1.0f, 0.0f); glVertex3f(x,y,z);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(x,y,       z + length);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y,z);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y,z + length);  
  glEnd();

  glBindTexture(GL_TEXTURE_2D, TexIDSkyBox[TOP_ID]);
  glBegin(GL_TRIANGLE_STRIP);                                   
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x,y + height,z);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y + height, z + length);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y + height, z + length);        
  glEnd();


  glBindTexture(GL_TEXTURE_2D, TexIDSkyBox[RIGHT_ID]);
  glBegin(GL_TRIANGLE_STRIP);                           
        glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y, z);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + height, z);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y, z + length);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y + height, z + length); 
  glEnd();

  glBindTexture(GL_TEXTURE_2D, TexIDSkyBox[LEFT_ID]);
  glBegin(GL_TRIANGLE_STRIP);                           
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y, z);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y, z + length);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height, z + length); 
  glEnd();

  glDepthMask(GL_TRUE);
  glEnable(GL_DEPTH_TEST);
}


void fix_endian_func(void* data, int size) {
        unsigned char* cdata = (unsigned char*) data;
        int i;
        for (i=0; i<size/2; i++) {
            unsigned char temp = cdata[i];
            cdata[i] = cdata[size-1 - i];
            cdata[size-1 - i] = temp;
        }
}
//-----------------------------------------------------------------------------
// Name: getBitmapImageData()
// Desc: Simply image loader for 24 bit BMP files.
// Shamelessly Stolen From: Nehe
//-----------------------------------------------------------------------------
bool getBitmapImageData(const char *pFileName, textureImage *pImage )
{
    FILE *pFile = NULL;
    unsigned short nNumPlanes;
    unsigned short nNumBPP;
    int i;
	int32_t t = 0;
    unsigned short t2 = 0;

	cout << endl;
	 
    if( (pFile = fopen(pFileName, "rb") ) == NULL )
       cout << "ERROR: getBitmapImageData - %s not found " << pFileName << "." << endl;

    // Seek forward to width and height info
    fseek( pFile, 18, SEEK_CUR );

    if( (i = fread(&t, 4, 1, pFile) ) != 1 )
       cout << "ERROR: getBitmapImageData - Couldn't read width from " << pFileName << "." << endl;
	fix_endian_func(&t, 4);
    printf("%08x\n", t);
	cout << "t: " << t << endl;
    pImage->width = t;
	cout << "width: " << pImage->width << endl;
	
    if( (i = fread(&t, 4, 1, pFile) ) != 1 )
       cout << "ERROR: getBitmapImageData - Couldn't read height from " << pFileName << "." << endl;
	fix_endian_func(&t, 4);
    pImage->height = t;
	cout << "height: " << pImage->height << endl;

    if( (fread(&t2, 2, 1, pFile) ) != 1 )
       cout << "ERROR: getBitmapImageData - Couldn't read plane count from " << pFileName << "." << endl;
	fix_endian_func(&t2, 2);
    nNumPlanes = t2;
	cout << "nNumPlanes: " << nNumPlanes << endl;
	
    if( nNumPlanes != 1 )
       cout << "ERROR: getBitmapImageData - Plane count from " << pFileName
            << " is not 1: " << nNumPlanes << "." << endl;

    if( (i = fread(&t2, 2, 1, pFile)) != 1 )
       cout << "ERROR: getBitmapImageData - Couldn't read BPP from " << pFileName << endl;
	fix_endian_func(&t2, 2);
    nNumBPP = t2;
	cout << "nNumBPP: " << nNumBPP << endl;

    if( nNumBPP != 24 )
       cout << "ERROR: getBitmapImageData - BPP from " << pFileName
            << " is not 24: " << nNumBPP << "." << endl;

    // Seek forward to image data
    fseek( pFile, 24, SEEK_CUR );

    // Calculate the image's total size in bytes. Note how we multiply the
    // result of (width * height) by 3. This is becuase a 24 bit color BMP
    // file will give you 3 bytes per pixel.
    int nTotalImagesize = (pImage->width * pImage->height) * 3;

    pImage->data = (char*) malloc( nTotalImagesize );

    if( (i = fread(pImage->data, 1, nTotalImagesize, pFile) ) != nTotalImagesize )
       cout << "ERROR: getBitmapImageData - Couldn't read image data from " << pFileName << "." << endl;

    //
    // Finally, rearrange BGR to RGB
    //

    int length = (pImage->width * 3); // + 3) & ~3;
	printf("length: %d\n", length);
	printf("image size: %d\n", nTotalImagesize);
	int x, y;
	char *ptr, temp;
    for (y = 0; y < pImage->height; y ++)
        for (ptr = pImage->data + y * length, x = pImage->width;
             x > 0;
	     x --, ptr += 3)
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
    return true;
}
