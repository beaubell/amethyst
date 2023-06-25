#ifndef AMETHYST_TYPES_H
#define AMETHYST_TYPES_H

#include <boost/multi_array.hpp>
#include <glm/glm.hpp>

typedef double float_type;
typedef float  glm_float_type;

using Am2DArrayUI = boost::multi_array<unsigned int , 2>;
using Am3DArrayUI = boost::multi_array<unsigned int , 3>;
using Am4DArrayUI = boost::multi_array<unsigned int , 4>;

using Am2DArrayF = boost::multi_array<float , 2>;
using Am3DArrayF = boost::multi_array<float , 3>;
using Am4DArrayF = boost::multi_array<float , 4>;

using Am2DArrayD = boost::multi_array<double , 2>;
using Am3DArrayD = boost::multi_array<double , 3>;
using Am4DArrayD = boost::multi_array<double , 4>;

typedef glm::mat4 TransMatrix;
typedef glm::mat3 NormalMatrix;
typedef glm::vec3 Vertex;
typedef glm::vec4 Vertex4;
typedef glm::vec2 TexCoord;
typedef glm::vec3 Normal;
typedef glm::vec4 Normal4;
typedef glm::vec4 Color;

#ifdef __GNUC__
#define PACK( __Declaration__ ) __Declaration__ __attribute__((__packed__))
#endif

#ifdef _MSC_VER
#define PACK( __Declaration__ ) __pragma( pack(push, 1) ) __Declaration__ __pragma( pack(pop))
#endif

#endif
