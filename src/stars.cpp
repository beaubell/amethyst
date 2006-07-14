/***************************************************************************
 *   Star Generation Routines                                              *
 *  (c) 2006 Beau V.C. Bellamy (beau@stellarnetservices.net)               *
 ***************************************************************************/

#include <stdio.h>  // fopen, scanf, printf, fclose
#include <stdlib.h> // malloc

#include <amethyst/physics.h>

#include "SDL_opengl.h"

#include "stars.h"

using namespace amethyst;

typedef struct
{
  float ra;
  float de;
  float bt;
  float vt;
  unsigned char spec;
  float alpha;
} _star_data;

// Storage object for starlist
_star_data *star;
int         entries;

static char spectral_class[7][4] =
{{'O',156,178,255}, // 30,000 - 60,000 K - Blue
 {'B',172,190,255},
 {'A',205,214,255},
 {'F',255,246,255},
 {'G',255,246,238},
 {'K',255,210,164},
 {'M',255,206,106}};



void load_stars(const char* filename)
{
    float V, BV;

    FILE *file = fopen(filename, "r");
    fscanf(file, "%d\n", &entries);
    printf("\nNumber of Star Entries: %d\n", entries);

    star = new _star_data[entries] ;

    for (unsigned int i = 0; i<entries; i++)
    {
       fscanf(file, "%f,%f,%f,%f\n", &star[i].ra, &star[i].de, &star[i].bt, &star[i].vt);

       //printf("%f,%f,%f,%f\n", star[2].ra, star[2].de, star[2].bt, star[2].vt);
       //fscanf(file, "%f,%f,%f,%f\n", &star.ra, &star.de, &star.bt, &star.vt);
       //printf("%f,%f,%f,%f\n", star.ra, star.de, star.bt, star.vt);

    // Determine Brightness from Magnitude Data
    V  = star[i].vt - 0.090f * ( star[i].bt - star[i].vt );
    star[i].alpha = 1 - ( (V-0.2f) /6.0);

    // Determine Spectral Class from Magnitude Data
    BV = 0.850f * (star[i].bt-star[i].vt);
    if( BV < -0.30)      // O
      star[i].spec = 0;
    else if (BV < 0.0)   // B
      star[i].spec = 1;
    else if (BV < 0.31)  // A
      star[i].spec = 2;
    else if (BV < 0.59)  // F
      star[i].spec = 3;
    else if (BV < 0.82)  // G
      star[i].spec = 4;
    else if (BV < 1.41)  // K
      star[i].spec = 5;
    else star[i].spec = 6; // M
    }

    fclose(file);
}


void display_stars()
{

    Cartesian_Vector temp;

    glPointSize(1);
    glEnable(GL_POINT_SMOOTH);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);



    glDisable(GL_LIGHTING);

    GLfloat fAmbLight[] =   { 0.05f, 0.05f, 0.05f, 1.00f }; // Black
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, fAmbLight);

    //glColorMaterial(GL_FRONT, GL_EMISSION);
    //glMateriali(GL_FRONT, GL_EMISSION, 128);

    glBegin(GL_POINTS);

    GLfloat alpha = 1;

    glColor3f(1.0f, 1.0f, 1.0f);

    for (unsigned int i = 0; i<entries; i++)
    {
        temp = phys_alias_transform (Spherical_Vector(star[i].ra, star[i].de, 10000));


        //if (star[i].bt < 4.5f) alpha = 1; else alpha = 0.1;
        glColor4ub(spectral_class[star[i].spec][1], spectral_class[star[i].spec][2], spectral_class[star[i].spec][3], (star[i].alpha * 255.0f));
        glVertex3d(temp.x,temp.y,temp.z);



    }

    glEnd();
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

    glEnable(GL_LIGHTING);
}
