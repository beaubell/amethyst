/***************************************************************************
 *   Model loading and processing function implimentations                 *
 *  (c) 2006 Beau V.C. Bellamy (beau@stellarnetservices.net)               *
 ***************************************************************************/

#include "model.h"

#include <math.h>    // cosf, sinf
#include <string>    // std::string

#include "file.h"
#include <stdlib.h>  // malloc
#include <errno.h>   // errno
#include <iostream>

#include "global.h"
#include "texture.h"

#define SPHERE_DETAIL 100


void models_load(void)
{
    // Load Ship Model
    Global.dlShip = glGenLists(1);
    glNewList(Global.dlShip, GL_COMPILE);
    model_load_file(Global.dir_models + "/" + "ship.mdl");  // FIXME Temp
    glEndList();


    // Load Sun Texture
    std::string sun = Global.dir_textures + "/" + Global.file_tex_sun;

    Global.sun_tex = image_load(sun.c_str());
    glBindTexture(GL_TEXTURE_2D, Global.sun_tex);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, 0x812F);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, 0x812F);

    // Load Sun Model
    Global.sun_mdl = glGenLists(1);
    glNewList(Global.sun_mdl, GL_COMPILE);
    glBindTexture(GL_TEXTURE_2D, Global.sun_tex);
    model_sphere_create(0,0,0,5000,SPHERE_DETAIL);
    glEndList();


    // Load Planet Texture
    std::string planet = Global.dir_textures + "/" + Global.file_tex_planet;

    Global.planet_tex = image_load(planet.c_str());
    glBindTexture(GL_TEXTURE_2D, Global.planet_tex);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, 0x812F);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, 0x812F);

    // Load Planet Model
    Global.planet_mdl = glGenLists(1);
    glNewList(Global.planet_mdl, GL_COMPILE);
    glBindTexture(GL_TEXTURE_2D, Global.planet_tex);
    model_sphere_create(0,0,0,6804900.0,SPHERE_DETAIL);
    glEndList();


    // Load Starfield
    std::string starfield = Global.dir_textures + "/" + Global.file_tex_starfield;

    Global.starfield_tex = image_load(starfield.c_str());
    glBindTexture(GL_TEXTURE_2D, Global.starfield_tex);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

        //glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, 0x812F);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, 0x812F);

    Global.starfield_mdl = glGenLists(1);
    glNewList(Global.starfield_mdl, GL_COMPILE);
    glBindTexture(GL_TEXTURE_2D, Global.starfield_tex);
    model_sphere_create(0,0,0,500000,20);
    glEndList();

}

void models_free(void)
{
    glDeleteLists(Global.dlShip,1);
    glDeleteLists(Global.starfield_mdl,1);
    glDeleteLists(Global.planet_mdl,1);
    glDeleteLists(Global.sun_mdl,1);

}

void model_sphere_create( float cx, float cy, float cz, float r, int p )
{
    const double TWOPI  = 6.28318530717958647692;
    const double PIDIV2 = 1.57079632679489661923;

    double theta1 = 0.0;
    double theta2 = 0.0;
    double theta3 = 0.0;

    double ex = 0.0f;
    double ey = 0.0f;
    double ez = 0.0f;

    double px = 0.0f;
    double py = 0.0f;
    double pz = 0.0f;

    // Disallow a negative number for radius.
    if( r < 0 )
        r = -r;

    // Disallow a negative number for precision.
    if( p < 0 )
        p = -p;

    // If the sphere is too small, just render a OpenGL point instead.
    if( p < 4 || r <= 0 )
    {
        glBegin( GL_POINTS );
        glVertex3d( cx, cy, cz );
        glEnd();
        return;
    }

    for( int i = 0; i < p/2; ++i )
    {
        theta1 = i * TWOPI / p - PIDIV2;
        theta2 = (i + 1) * TWOPI / p - PIDIV2;

        glBegin( GL_TRIANGLE_STRIP );
        {
            for( int j = p; j >= 0; --j )
            {
                theta3 = j * TWOPI / p;

                ex = cosf(theta2) * cosf(theta3);
                ey = sinf(theta2);
                ez = cosf(theta2) * sinf(theta3);
                px = cx + r * ex;
                py = cy + r * ey;
                pz = cz + r * ez;

                glNormal3d( ex, ey, ez );
                glTexCoord2d( -(j/(double)p) , 2*(i+1)/(double)p );
                glVertex3d( px, py, pz );

                ex = cosf(theta1) * cosf(theta3);
                ey = sinf(theta1);
                ez = cosf(theta1) * sinf(theta3);
                px = cx + r * ex;
                py = cy + r * ey;
                pz = cz + r * ez;

                glNormal3d( ex, ey, ez );
                glTexCoord2d( -(j/(double)p), 2*i/(double)p );
                glVertex3d( px, py, pz );
            }
        }
        glEnd();
    }
}


Model* model_load_file(const std::string &filename)
{
    if (access(filename.c_str(), R_OK) < 0) {
        if (errno == ENOENT)
            printf ("Model file doesn't exist: %s\n", filename.c_str());
        if (errno == EACCES)
            printf ("Access denied accessing model file: %s\n", filename.c_str());
        return NULL;
    }

    unsigned int vertices = 0, vertices_t = 0, i = 0;
    FILE *file = fopen(filename.c_str(), "r");

    fscanf(file, "%d\n", &vertices);

    Model *model = new Model;
    model->data = new float[vertices * 8];

    //FIXME TEMP
    glBegin(GL_TRIANGLES);

    vertices_t = vertices * 8;

    for (i = 0; !feof(file), i < vertices_t; i += 8)
    {
        fscanf(file, "%f, %f,",      &model->data[i],   &model->data[i+1]);
        fscanf(file, "%f, %f, %f,",  &model->data[i+2], &model->data[i+3], &model->data[i+4]);
        fscanf(file, "%f, %f, %f\n", &model->data[i+5], &model->data[i+6], &model->data[i+7]);

        // FIXME Temporary
        //glTexCoord2fv(&(model->data[i]));
        glNormal3fv(&model->data[i+2]);
        glVertex3fv(&(model->data[i+5]));
    }

    //FIXME TEMP
    glEnd();

    if (i != vertices_t) {
        printf("ERROR: Model file appears corrupted: Got (%i), expected (%i)\n", i, vertices_t);
        vertices = i;
    }

    return model;
}
