/***************************************************************************
 *   Star Generation Routines                                              *
 *  (c) 2006 Beau V.C. Bellamy (beau@stellarnetservices.net)               *
 ***************************************************************************/

#include "file.h"
#include <stdlib.h> // malloc
#include <errno.h>  //
#include <iostream>

#include "lib/physics.h"

#include "SDL_opengl.h"

#include "stars.h"

using namespace amethyst;

typedef struct
{
  float ra;
  float de;
  float bt;
  float vt;
  float x,y,z;
  unsigned char type;
  float alpha;
} _star_data;

typedef struct
{
  GLubyte r;
  GLubyte g;
  GLubyte b;
  GLubyte a;
  GLfloat x;
  GLfloat y;
  GLfloat z;
} _star_vertex;

// Storage object for starlist
_star_data   *star;
_star_vertex *star_vertex;
unsigned int  entries;


static unsigned char spectral_class[7][4] =
{{'O',156,178,255}, // 30,000 - 60,000 K - Blue
 {'B',172,190,255},
 {'A',205,214,255},
 {'F',255,246,255},
 {'G',255,246,238},
 {'K',255,210,164},
 {'M',255,206,106}};




void stars_load(std::string &filestr)
{
    float V, BV;
    const char *filename = filestr.c_str();

    if (access(filename, R_OK) < 0) {
       if (errno == ENOENT)
          printf ("Star file doesn't exist: %s\n", filename);
       if (errno == EACCES)
          printf ("Access denied accessing star file: %s\n", filename);
       return;
    }

    FILE *file = fopen(filename, "r");
    fscanf(file, "%d\n", &entries);

    star = new _star_data[entries] ;

    unsigned int i;

    for (i = 0; !feof(file), i<entries; i++)
    {
       fscanf(file, "%f,%f,%f,%f\n", &star[i].ra, &star[i].de, &star[i].bt, &star[i].vt);

       //printf("%f,%f,%f,%f\n", star[2].ra, star[2].de, star[2].bt, star[2].vt);

       // Determine Brightness from Magnitude Data
       V  = star[i].vt - 0.090f * ( star[i].bt - star[i].vt );
       star[i].alpha = 1.0f - ( (V-0.2f) /6.0f);

       // Determine Spectral Class from Magnitude Data
       BV = 0.850f * (star[i].bt-star[i].vt);
       if( BV < -0.30)      // O
         star[i].type = 0;
       else if (BV < 0.0)   // B
         star[i].type = 1;
       else if (BV < 0.31)  // A
         star[i].type = 2;
       else if (BV < 0.59)  // F
         star[i].type = 3;
       else if (BV < 0.82)  // G
         star[i].type = 4;
       else if (BV < 1.41)  // K
         star[i].type = 5;
       else star[i].type = 6; // M
    }

    if (i != entries) {
      printf("ERROR: Star file appears corrupted: Got (%i), expected (%i)\n", i, entries);
       entries = i;
    }

    printf("Loaded %d stars from file: %s\n", entries, filename);

    //Create Display List
    star_vertex = new _star_vertex[entries];

    Cartesian_Vector temp;

    for (unsigned int i = 0; i<entries; i++)
    {
        temp = phys_alias_transform (Spherical_Vector(star[i].ra, star[i].de, 10000));

        star_vertex[i].r = spectral_class[star[i].type][1];
        star_vertex[i].g = spectral_class[star[i].type][2];
        star_vertex[i].b = spectral_class[star[i].type][3];
        star_vertex[i].a = (unsigned char)(star[i].alpha * 255.0f);

        star_vertex[i].x = (float)temp.x;
        star_vertex[i].y = (float)temp.y;
        star_vertex[i].z = (float)temp.z;
    }

    fclose(file);
}


void stars_free(void)
{
    delete[] star;
    delete[] star_vertex;
    entries = 0;
}


void stars_render()
{
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glPointSize(2);

    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);

    glEnableClientState(GL_VERTEX_ARRAY);
    glInterleavedArrays(GL_C4UB_V3F, 0, star_vertex);
    glDrawArrays(GL_POINTS, 0 ,entries);

    glPopAttrib();
}
