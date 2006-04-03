/***************************************************************************
 *   Texture loading and processing function implimentation                *
 *   (c) 2006 Beau V.C. Bellamy (beau@stellarnetservices.net)              *
 ***************************************************************************/



#include "SDL/SDL_image.h"
#include "SDL/SDL_opengl.h"


#include <amethyst/object.h>
#include <iostream>

#include <stdio.h>

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

GLuint load_image(const char *file) {

    GLuint texture;

    //SDL_Surface *image, *test;
    //SDL_RWops *rwop;

    // Open File "file"
    //rwop=SDL_RWFromFile(file, "rb");

    // Load Image from 'rwop' ("file")
    //image=IMG_LoadJPG_RW(rwop);

    textureImage *texti;

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


//    std::cout << "\nSDL Surface" << std::endl;
//    std::cout << " Flags    : "  << image->flags << std::endl;
//    std::cout << " Format"       << std::endl;
//    std::cout << "  BitsPP  : "  << image->format->BitsPerPixel  << std::endl;
//    std::cout << "  BytesPP : "  << image->format->BytesPerPixel << std::endl;
//    std::cout << " W/H      : "  << image->w << "/" << image->h << std::endl;
//    std::cout << " Pitch    : "  << image->pitch << std::endl;

     // Free the surface that SDL created for us when it loaded the image
     //SDL_FreeSurface(image);
     free (texti->data);
     free (texti);
     return texture;

    }

void load_skybox (void)
{
    int i=1;

    // bmp test loader
    //textureImage *texti;

    //glGenTextures(1, &TexIDSkyBox[0]);
    //texti = (textureImage *)malloc(sizeof(textureImage));
    //getBitmapImageData("/home/beau/projects/skybox/10/bk.bmp", texti);
    //glBindTexture(GL_TEXTURE_2D, TexIDSkyBox[0]);
    //glTexImage2D(GL_TEXTURE_2D, 0, 3, texti->width, texti->height, 0, GL_RGB, GL_UNSIGNED_BYTE, texti->data);
    

    TexIDSkyBox[0] = load_image("/home/beau/.amethyst/textures/bk.bmp");
    TexIDSkyBox[1] = load_image("/home/beau/.amethyst/textures/ft.bmp");
    TexIDSkyBox[2] = load_image("/home/beau/.amethyst/textures/dn.bmp");
    TexIDSkyBox[3] = load_image("/home/beau/.amethyst/textures/up.bmp");
    TexIDSkyBox[4] = load_image("/home/beau/.amethyst/textures/rt.bmp");
    TexIDSkyBox[5] = load_image("/home/beau/.amethyst/textures/lt.bmp");

    


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

    // Lit texture environment
    //glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    // Load the ship texture
    //GLint iWidth, iHeight,iComponents;
    //GLenum eFormat;
    //GLbyte *pBytes = gltLoadTGA("YellowSub.tga", &iWidth, &iHeight, &iComponents, &eFormat);
    //glTexImage2D(GL_TEXTURE_2D, 0, iComponents, iWidth, iHeight, 0, eFormat, GL_UNSIGNED_BYTE, (void *)pBytes);
    //free(pBytes);

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

/* simple loader for 24bit bitmaps (data is in rgb-format) shamelessly borrowed from Nehe */
int loadBMP(const char *filename, textureImage *texture)
{
    FILE *file;
    unsigned short int bfType;
    long int bfOffBits;
    short int biPlanes;
    short int biBitCount;
    long int biSizeImage;
    int i;
    unsigned char temp;
    /* make sure the file is there and open it read-only (binary) */
    if ((file = fopen(filename, "rb")) == NULL)
    {
        printf("File not found : %s\n", filename);
        return 0;
    }
    if(!fread(&bfType, sizeof(short int), 1, file))
    {
        printf("Error reading file!\n");
        return 0;
    }
    /* check if file is a bitmap */
    if (bfType != 19778)
    {
        printf("Not a Bitmap-File!\n");
        return 0;
    }        
    /* get the file size */
    /* skip file size and reserved fields of bitmap file header */
    fseek(file, 8, SEEK_CUR);
    /* get the position of the actual bitmap data */
    if (!fread(&bfOffBits, sizeof(long int), 1, file))
    {
        printf("Error reading file!\n");
        return 0;
    }
    printf("Data at Offset: %ld\n", bfOffBits);
    /* skip size of bitmap info header */
    fseek(file, 4, SEEK_CUR);
    /* get the width of the bitmap */
    fread(&texture->width, sizeof(int), 1, file);
    printf("Width of Bitmap: %d\n", texture->width);
    /* get the height of the bitmap */
    fread(&texture->height, sizeof(int), 1, file);
    printf("Height of Bitmap: %d\n", texture->height);
    /* get the number of planes (must be set to 1) */
    fread(&biPlanes, sizeof(short int), 1, file);
    if (biPlanes != 1)
    {
        printf("Error: number of Planes not 1!\n");
        return 0;
    }
    /* get the number of bits per pixel */
    if (!fread(&biBitCount, sizeof(short int), 1, file))
    {
        printf("Error reading file!\n");
        return 0;
    }
    printf("Bits per Pixel: %d\n", biBitCount);
    if (biBitCount != 24)
    {
        printf("Bits per Pixel not 24\n");
       return 0;
    }
    /* calculate the size of the image in bytes */
    biSizeImage = texture->width * texture->height * 3;
    printf("Size of the image data: %ld\n", biSizeImage);
    texture->data = (unsigned char*)malloc(biSizeImage);
    /* seek to the actual data */
    fseek(file, bfOffBits, SEEK_SET);
    if (!fread(texture->data, biSizeImage, 1, file))
    {
        printf("Error loading file!\n");
        return 0;
    }
    /* swap red and blue (bgr -> rgb) */
    for (i = 0; i < biSizeImage; i += 3)
    {
        temp = texture->data[i];
        texture->data[i] = texture->data[i + 2];
        texture->data[i + 2] = temp;
    }
    return 1;
}


//-----------------------------------------------------------------------------
// Name: getBitmapImageData()
// Desc: Simply image loader for 24 bit BMP files.
//-----------------------------------------------------------------------------
bool getBitmapImageData(const char *pFileName, textureImage *pImage )
{
    FILE *pFile = NULL;
    unsigned short nNumPlanes;
    unsigned short nNumBPP;
	int i;

    if( (pFile = fopen(pFileName, "rb") ) == NULL )
		cout << "ERROR: getBitmapImageData - %s not found " << pFileName << "." << endl;

    // Seek forward to width and height info
    fseek( pFile, 18, SEEK_CUR );

    if( (i = fread(&pImage->width, 4, 1, pFile) ) != 1 )
		cout << "ERROR: getBitmapImageData - Couldn't read width from " << pFileName << "." << endl; 

    if( (i = fread(&pImage->height, 4, 1, pFile) ) != 1 )
		cout << "ERROR: getBitmapImageData - Couldn't read height from " << pFileName << "." << endl; 

    if( (fread(&nNumPlanes, 2, 1, pFile) ) != 1 )
		cout << "ERROR: getBitmapImageData - Couldn't read plane count from " << pFileName << "." << endl; 
	
    if( nNumPlanes != 1 )
		cout << "ERROR: getBitmapImageData - Plane count from " << pFileName << " is not 1: " << nNumPlanes << "." << endl;

    if( (i = fread(&nNumBPP, 2, 1, pFile)) != 1 )
		cout << "ERROR: getBitmapImageData - Couldn't read BPP from " << pFileName << endl; 
	
    if( nNumBPP != 24 )
		cout << "ERROR: getBitmapImageData - BPP from " << pFileName << " is not 24: " << nNumBPP << "." << endl;

    // Seek forward to image data
    fseek( pFile, 24, SEEK_CUR );

	// Calculate the image's total size in bytes. Note how we multiply the 
	// result of (width * height) by 3. This is becuase a 24 bit color BMP 
	// file will give you 3 bytes per pixel.
    int nTotalImagesize = (pImage->width * pImage->height) * 3;

    pImage->data = (unsigned char*) malloc( nTotalImagesize );
	
    if( (i = fread(pImage->data, nTotalImagesize, 1, pFile) ) != 1 )
		cout << "ERROR: getBitmapImageData - Couldn't read image data from " << pFileName << "." << endl;

    //
	// Finally, rearrange BGR to RGB
	//
	
	char charTemp;
    for( i = 0; i < nTotalImagesize; i += 3 )
	{ 
		charTemp = pImage->data[i];
		pImage->data[i] = pImage->data[i+2];
		pImage->data[i+2] = charTemp;
    }

    return true;
}