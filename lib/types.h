#ifndef AMETHYST_TYPES_H
#define AMETHYST_TYPES_H

#include "cl.h"
#include <boost/multi_array.hpp>
#include <glm/glm.hpp>

typedef cl_double float_type;

typedef boost::multi_array<unsigned int , 2> Am2DArrayUI;
typedef boost::multi_array<unsigned int , 3> Am3DArrayUI;
typedef boost::multi_array<unsigned int , 4> Am4DArrayUI;

typedef boost::multi_array<float , 2> Am2DArrayF;
typedef boost::multi_array<float , 3> Am3DArrayF;
typedef boost::multi_array<float , 4> Am4DArrayF;

typedef boost::multi_array<double , 2> Am2DArrayD;
typedef boost::multi_array<double , 3> Am3DArrayD;
typedef boost::multi_array<double , 4> Am4DArrayD;

typedef glm::mat4 TransMatrix;
typedef glm::mat3 NormalMatrix;
typedef glm::vec3 Vertex;
typedef glm::vec4 Vertex4;
typedef glm::vec2 TexCoord;
typedef glm::vec3 Normal;
typedef glm::vec4 Normal4;
typedef glm::vec4 Color;
#endif