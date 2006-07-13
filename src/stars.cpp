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
} _star_data;

// Storage object for starlist
_star_data *star;
int         entries;

void load_stars(const char* filename)
{

    FILE *file = fopen(filename, "r");
    fscanf(file, "%d\n", &entries);
    printf("\nNumber of Star Entries: %d\n", entries);

    star = new _star_data[10000] ;

    for (unsigned int i = 0; i<entries; i++)
       fscanf(file, "%f,%f,%f,%f\n", &star[i].ra, &star[i].de, &star[i].bt, &star[i].vt);

    //printf("%f,%f,%f,%f\n", star[2].ra, star[2].de, star[2].bt, star[2].vt);
    //fscanf(file, "%f,%f,%f,%f\n", &star.ra, &star.de, &star.bt, &star.vt);
    //printf("%f,%f,%f,%f\n", star.ra, star.de, star.bt, star.vt);

    fclose(file);
}


void display_stars()
{

    Cartesian_Vector temp;

    //glPointSize(4);
    //glEnable(GL_POINT_SMOOTH);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);



    glDisable(GL_LIGHTING);

    //GLfloat fAmbLight[] =   { 1.00f, 1.00f, 1.00f, 1.00f }; // Black
    //glLightModelfv(GL_LIGHT_MODEL_AMBIENT, fAmbLight);

    //glColorMaterial(GL_FRONT, GL_EMISSION);
    //glMateriali(GL_FRONT, GL_EMISSION, 128);

    glBegin(GL_POINTS);


    glColor3f(1.0f, 0.0f, 0.0f);

    for (unsigned int i = 0; i<entries; i++)
    {
        temp = phys_alias_transform (Spherical_Vector(star[i].ra, star[i].de, 10000));
        
        glVertex3d(temp.x,temp.y,temp.z);


    }

    glEnd();


    glEnable(GL_LIGHTING);
}
