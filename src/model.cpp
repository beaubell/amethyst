/***************************************************************************
 *   Model loading and processing function implimentations                 *
 *  (c) 2006 Beau V.C. Bellamy (beau@stellarnetservices.net)               *
 ***************************************************************************/

#include "model.h"

#include <math.h>    // cosf, sinf
#include <string>    // std::string

#include <stdio.h>   // fopen, scanf, printf, fclose, feof
#include <stdlib.h>  // malloc
#include <errno.h>   // errno
#include <iostream>

#include "global.h"
#include "texture.h"

#define SPHERE_DETAIL 100


void load_models(void)
{
    // Load Ship Model
    Global.dlShip = glGenLists(1);
    glNewList(Global.dlShip, GL_COMPILE);
    load_model_file(Global.dir_models + "/" + "ship.mdl");  // FIXME Temp
    glEndList();


    // Load Sun Texture
    std::string sun = Global.dir_textures + "/" + Global.file_tex_sun;

    Global.sun_tex = load_image(sun.c_str());
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
    renderSphere(0,0,0,5000,SPHERE_DETAIL);
    glEndList();


    // Load Planet Texture
    std::string planet = Global.dir_textures + "/" + Global.file_tex_planet;

    Global.planet_tex = load_image(planet.c_str());
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
    renderSphere(0,0,0,50,SPHERE_DETAIL);
    glEndList();


    // Load Starfield
    std::string starfield = Global.dir_textures + "/" + Global.file_tex_starfield;

    Global.starfield_tex = load_image(starfield.c_str());
    glBindTexture(GL_TEXTURE_2D, Global.starfield_tex);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

        //glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, 0x812F);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, 0x812F);

    Global.starfield_mdl = glGenLists(1);
    glNewList(Global.starfield_mdl, GL_COMPILE);
    glBindTexture(GL_TEXTURE_2D, Global.starfield_tex);
    renderSphere(0,0,0,500000,20);
    glEndList();

}

void free_models(void)
{
    glDeleteLists(Global.dlShip,1);
    glDeleteLists(Global.starfield_mdl,1);
    glDeleteLists(Global.planet_mdl,1);
    glDeleteLists(Global.sun_mdl,1);

}

void renderSphere( float cx, float cy, float cz, float r, int p )
{
    const float TWOPI  = 6.28318530717958f;
    const float PIDIV2 = 1.57079632679489f;

    float theta1 = 0.0;
    float theta2 = 0.0;
    float theta3 = 0.0;

    float ex = 0.0f;
    float ey = 0.0f;
    float ez = 0.0f;

    float px = 0.0f;
    float py = 0.0f;
    float pz = 0.0f;

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
        glVertex3f( cx, cy, cz );
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

                glNormal3f( ex, ey, ez );
                glTexCoord2f( -(j/(float)p) , 2*(i+1)/(float)p );
                glVertex3f( px, py, pz );

                ex = cosf(theta1) * cosf(theta3);
                ey = sinf(theta1);
                ez = cosf(theta1) * sinf(theta3);
                px = cx + r * ex;
                py = cy + r * ey;
                pz = cz + r * ez;

                glNormal3f( ex, ey, ez );
                glTexCoord2f( -(j/(float)p), 2*i/(float)p );
                glVertex3f( px, py, pz );
            }
        }
        glEnd();
    }
}


Model* load_model_file(const std::string &filename)
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

    for (i = 0; i < vertices, !feof(file); i += 8)
    {
        fscanf(file, "%f, %f,",      &model->data[i],   &model->data[i+1]);
        fscanf(file, "%f, %f, %f,",  &model->data[i+2], &model->data[i+3], &model->data[i+4]);
        fscanf(file, "%f, %f, %f\n", &model->data[i+5], &model->data[i+6], &model->data[i+7]);

        // FIXME Temporary
        glTexCoord2fv(&(model->data[i]));
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
