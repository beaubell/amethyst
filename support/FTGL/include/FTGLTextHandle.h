#ifndef     __FTGLTextHandle__
#define     __FTGLTextHandle__

#include <vector>
#include "glm/glm.hpp"

namespace ftgl {

class TextHandle
{
public:
    TextHandle();
   ~TextHandle();
    void render();
    void bind(int program, int vertexloc, int texcoordloc, int texUnitLoc, int normalloc);
    void clear();

    unsigned int texture;
    unsigned int num_chars;
    unsigned int _vao[1]; // Vertex Array Objects Identifier
    unsigned int _buffer[3]; // 0=quads, 1=textures
    
    int _programloc;
    int _vertexloc;
    int _texcoordloc;
    int _normalloc;
    int _texUnitLoc;
    
    std::vector<glm::vec4> vertex;
    std::vector<glm::vec2> texcoords;

    void addTriangle(unsigned int newtexture, 
		     const glm::vec4 &vert1, const glm::vec2 &texcoord1,
		     const glm::vec4 &vert2, const glm::vec2 &texcoord2,
		     const glm::vec4 &vert3, const glm::vec2 &texcoord3);
    void addVertex(const glm::vec4 &newvert, const glm::vec2 &newtexcoord);
};

} //namespace ftgl

#endif // __FTGLTextHandle__