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

void load_image(void) {


    SDL_Surface *test = IMG_Load("/home/beau/projects/skybox/10/ft.jpg");

    std::cout << "\nSDL Surface" << std::endl;
    std::cout << " Flags    : "  << test->flags << std::endl;
    std::cout << " Format"     << std::endl;
    std::cout << "  BitsPP  : "  << (unsigned char)((test->format)->BitsPerPixel)  << std::endl;
    std::cout << "  BytesPP : "  << test->format->BytesPerPixel << std::endl;
    std::cout << " W/H      : "  << test->w << "/" << test->h << std::endl;
    std::cout << " Pitch    : "  << test->pitch << std::endl;

    glGenTextures(1, &Global.texture[0]);   /* create the texture */
    glBindTexture(GL_TEXTURE_2D, Global.texture[0]);
    /* actually generate the texture */
    glTexImage2D(GL_TEXTURE_2D, 0, 3, test->w, test->h, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, test->pixels);
    /* enable linear filtering */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }