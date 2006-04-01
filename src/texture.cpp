/***************************************************************************
 *   Texture loading and processing function implimentation                *
 *   (c) 2006 Beau V.C. Bellamy (beau@stellarnetservices.net)              *
 ***************************************************************************/



#include "SDL/SDL_image.h"
#include "SDL/SDL_opengl.h"
#include <amethyst/object.h>
#include <iostream>

using namespace std;
using namespace amethyst;

class __global {

  public:
    __global() { ship.location.x = 10;
      ship.location.y = 0;
      ship.location.z = 0;

      ship.attitude.w = 1;
      ship.attitude.x = 0;
      ship.attitude.y = 0;
      ship.attitude.z = 0;

      cam_num = 0;};

      // Your Ship
      Object ship;

      // Pointer to quadratic
      GLUquadric *quadratic;

      // Storage for one texture
      GLuint  texture[1];

      // Joystick
      //joystick *

      // Camera Ring Buffer
      Cartesian_Vector cam_pos[10], cam_view[10], cam_up[10];
      int cam_num;

};

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

    SDL_Surface *image, *test;
    SDL_RWops *rwop;
    rwop=SDL_RWFromFile(file, "rb");
    image=IMG_LoadJPG_RW(rwop);

//    test=image;

    glGenTextures(1, &texture);
    glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
    glBindTexture(GL_TEXTURE_2D, texture);
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, image->w,image->h,GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);


//    SDL_Surface *test = IMG_Load("/home/beau/projects/skybox/10/ft.jpg");

//    std::cout << "\nSDL Surface" << std::endl;
//    std::cout << " Flags    : "  << test->flags << std::endl;
//    std::cout << " Format"     << std::endl;
//    std::cout << "  BitsPP  : "  << test->format->BitsPerPixel  << std::endl;
//    std::cout << "  BytesPP : "  << test->format->BytesPerPixel << std::endl;
//    std::cout << " W/H      : "  << test->w << "/" << test->h << std::endl;
//    std::cout << " Pitch    : "  << test->pitch << std::endl;


//    glGenTextures(1, &Global.texture[0]);   /* create the texture */
//    glBindTexture(GL_TEXTURE_2D, Global.texture[0]);
    /* actually generate the texture */
//    glTexImage2D(GL_TEXTURE_2D, 0, 4, test->w, test->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, test->pixels);
    /* enable linear filtering */
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
     SDL_FreeSurface(image);
     return texture;

    }

void load_skybox (void)
{
    int i=1;

    TexIDSkyBox[0] = load_image("/home/beau/projects/skybox/10/bk.jpg");
    TexIDSkyBox[1] = load_image("/home/beau/projects/skybox/10/ft.jpg");
    TexIDSkyBox[2] = load_image("/home/beau/projects/skybox/10/up.jpg");
    TexIDSkyBox[3] = load_image("/home/beau/projects/skybox/10/dn.jpg");
    TexIDSkyBox[4] = load_image("/home/beau/projects/skybox/10/rt.jpg");
    TexIDSkyBox[5] = load_image("/home/beau/projects/skybox/10/lt.jpg");

    for(i=0;i<=5;++i)
    {
        glBindTexture(GL_TEXTURE_2D, TexIDSkyBox[i]);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, 0x812F);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, 0x812F);
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