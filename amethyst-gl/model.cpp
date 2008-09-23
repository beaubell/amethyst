/***********************************************************************
 Amethyst-GL
  - Model loading and processing function implementations

 Authors (c):
 2004-2008 Beau V.C. Bellamy (beau@stellarnetservices.net)

 $Revision$
 $LastChangedDate$
 $LastChangedBy$
 ***********************************************************************/

#include "model.h"
#include "model_xml.h"

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

    // Load Sun Texture
    std::string sun = Global.dir_textures + "/" + Global.file_tex_sun;

    Global.sun_tex = image_load(sun.c_str());
    glBindTexture(GL_TEXTURE_2D, Global.sun_tex);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    // Load Sun Model
    Global.sun_mdl = glGenLists(1);
    glNewList(Global.sun_mdl, GL_COMPILE);
    glBindTexture(GL_TEXTURE_2D, Global.sun_tex);
    model_sphere_create(0,0,0,5000,SPHERE_DETAIL);
    glEndList();

}

Model* model_load(std::string &model_name)
{

    Model *model = NULL;

    //FIXME XXX Interate through list then return handle if found

    //FIXME XXX else add to list,
    {
        model = new Model;
        model->dl = 0;
        if (!model_xml_load(model_name, *model))
        {
            delete model;
            model = NULL;
        } else
        {
            //FIXME  XXX Add to licked list.
        }
    }

    return model;
}

void models_free(void)
{
    glDeleteLists(Global.sun_mdl,1);

}

void model_sphere_create(const double cx, const double cy, const double cz, double r, int p )
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

                ex = cos(theta2) * cos(theta3);
                ey = sin(theta2);
                ez = cos(theta2) * sin(theta3);
                px = cx + r * ex;
                py = cy + r * ey;
                pz = cz + r * ez;

                glNormal3d( ex, ey, ez );
                glTexCoord2d( -(j/(double)p) , 2*(i+1)/(double)p );
                glVertex3d( px, py, pz );

                ex = cos(theta1) * cos(theta3);
                ey = sin(theta1);
                ez = cos(theta1) * sin(theta3);
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
