/***********************************************************************
 Amethyst-GL
  - Starfield rendering implementation

 Authors (c):
 2006-2020 Beau V.C. Bellamy (bellamy.beau@gmail.com)
 ***********************************************************************/

#include "stars.h"
#include "opengl.h"
#include "global.h"

#include "file.h"

#include "lib/physics.h"
#include "shaderprog.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>

#include <stdlib.h> // malloc
#include <unistd.h>
#include <errno.h>  //
#include <iostream>
#include <cmath>

extern const char _binary_amethyst_gl_shaders_baseline_vert_start, _binary_amethyst_gl_shaders_baseline_vert_end;
extern const char _binary_amethyst_gl_shaders_baseline_frag_start, _binary_amethyst_gl_shaders_baseline_frag_end;

namespace amethyst {
namespace client {

using lib::Cartesian_Vector;
using lib::Spherical_Vector;


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
  GLfloat x;
  GLfloat y;
  GLfloat z;
  GLfloat w;
} _star_vertex;

typedef struct
{
  GLfloat r;
  GLfloat g;
  GLfloat b;
  GLfloat a;
} _star_color;

// Storage object for starlist
_star_data   *star;
_star_vertex *star_vertex;
_star_color  *star_color;

unsigned int  entries;

// Shader Program Location
ShaderProgram::sptr star_shader;

// Vertex Attribute Locations
ShaderProgram::AttribHDL vertexLoc, colorLoc;
 
// Uniform variable Locations
ShaderProgram::UniformHDL projMatrixLoc, viewMatrixLoc;

// Vertex Array Objects Identifiers
GLuint vao[1];

static GLfloat spectral_class[7][4] =
{{'O',0.61,0.69,1.00}, // 30,000 - 60,000 K - Blue
 {'B',0.67,0.74,1.00},
 {'A',0.80,0.83,1.00},
 {'F',1.00,0.96,1.00},
 {'G',1.00,0.96,0.93},
 {'K',1.00,0.82,0.64},
 {'M',1.00,0.80,0.41}};

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

    for (i = 0; !feof(file) && i<entries; i++)
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
    star_color = new _star_color[entries];

    Cartesian_Vector temp;

    for (unsigned int i = 0; i<entries; i++)
    {
        temp = lib::phys_alias_transform (Spherical_Vector(star[i].ra*M_PI/180.0, (star[i].de + 90.0)*M_PI/180.0, 10000.0));

        star_color[i].r = spectral_class[star[i].type][1];
        star_color[i].g = spectral_class[star[i].type][2];
        star_color[i].b = spectral_class[star[i].type][3];
        star_color[i].a = star[i].alpha;

        star_vertex[i].x = (float)temp.x;
        star_vertex[i].y = (float)temp.y;
        star_vertex[i].z = (float)temp.z;
	star_vertex[i].w = 1.0;
    }

    fclose(file);

    // Setup Shaders;
    star_shader = std::make_shared<ShaderProgram>(LOAD_RESOURCE(amethyst_gl_shaders_baseline_vert, Global.dir_shaders, "baseline.vert"),
                                                  LOAD_RESOURCE(amethyst_gl_shaders_baseline_frag, Global.dir_shaders, "baseline.frag"));

    vertexLoc = star_shader->GetAttribLocation("positionData");
    colorLoc  = star_shader->GetAttribLocation("colorData"); 
    projMatrixLoc = star_shader->GetUniformLocation("projMatrix");
    viewMatrixLoc = star_shader->GetUniformLocation("viewMatrix");
    
    //glBindFragDataLocation(star_shader_program, 0, "outputF");
    std::cout << "vectexLoc:" << vertexLoc.value << ", colorLoc:" << colorLoc.value << std::endl;
    std::cout << "projMatrixLoc:" << projMatrixLoc.value << ", viewMatrixLoc:" << viewMatrixLoc.value << std::endl;
    GLuint buffers[2];
 
    glGenVertexArrays(1, vao);

    glBindVertexArray(vao[0]);

    // Generate two slots for the vertex and color buffers
    glGenBuffers(2, buffers);
    
    
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(_star_vertex)*entries, star_vertex, GL_STATIC_DRAW);
    glEnableVertexAttribArray(vertexLoc.value);
    glVertexAttribPointer(vertexLoc.value, 4, GL_FLOAT, 0, 0, 0);

    // bind buffer for colors and copy data into buffer
    glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(_star_color)*entries, star_color, GL_STATIC_DRAW);
    glEnableVertexAttribArray(colorLoc.value);
    glVertexAttribPointer(colorLoc.value, 4, GL_FLOAT, 0, 0, 0);
}


void stars_free(void)
{
    delete[] star;
    delete[] star_vertex;
    delete[] star_color;
    entries = 0;

    star_shader = NULL;
}


void stars_render(const glm::mat4 &projMatrix, const glm::mat4 &viewMatrix)
{
    glPointSize(2);

    glDisable(GL_DEPTH_TEST);

    star_shader->use();
    star_shader->UniformMatrix4f(projMatrixLoc, projMatrix);
    star_shader->UniformMatrix4f(viewMatrixLoc, viewMatrix);
    
    glBindVertexArray(vao[0]);
    glDrawArrays(GL_POINTS, 0, entries);
    
    glEnable(GL_DEPTH_TEST);
}

} // namespace client
} // namespace amethyst
