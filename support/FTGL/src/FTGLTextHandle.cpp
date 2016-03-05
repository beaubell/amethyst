#include "FTGLTextHandle.h"

#define GL_GLEXT_PROTOTYPES true
#include "GL/glcorearb.h"

namespace ftgl {

TextHandle::TextHandle()
 :texture(0),
  num_chars(0),
  _programloc(-1),
  _vertexloc(-1),
  _texcoordloc(-1),
  _normalloc(-1)
{
    _vao[0] = -1; 
    _buffer[0] = -1; // 0=triangles, 1=textures, 2=normals
    _buffer[1] = -1; // 0=triangles, 1=textures, 2=normals
    _buffer[2] = -1; // 0=triangles, 1=textures, 2=normals
}

TextHandle::~TextHandle()
{
    // Delete vao objects
  
}

void TextHandle::render()
{
    if(_programloc == -1)
      return;

    glUseProgram(_programloc);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture( GL_TEXTURE_2D, texture);

    // Render vao objects
    glBindVertexArray(_vao[0]);
    glDrawArrays(GL_TRIANGLES, 0, vertex.size());
}

void TextHandle::bind(int program, int vertexloc, int texcoordloc, int texUnitLoc, int normalloc)
{
    _programloc  = program;
    _vertexloc   = vertexloc;
    _texcoordloc = texcoordloc;
    _normalloc   = normalloc;
    _texUnitLoc  = texcoordloc;

    // Generate handle for Vertex Array is doesn't exist
    if (_vao[0] == -1)
      glGenVertexArrays(1, _vao);

    glBindVertexArray(_vao[0]);

    // Generate handle for buffers if don't exist;
    if (_buffer[0]== -1)
      glGenBuffers(3, _buffer);

    glBindBuffer(GL_ARRAY_BUFFER, _buffer[0]);
    glBufferData(GL_ARRAY_BUFFER, vertex.size()*sizeof(float)*4, &vertex[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(vertexloc);
    glVertexAttribPointer(vertexloc, 4, GL_FLOAT, 0, 0, 0);
    
    if (texcoordloc > -1)
    {
      glBindBuffer(GL_ARRAY_BUFFER, _buffer[1]);
      glBufferData(GL_ARRAY_BUFFER, texcoords.size()*sizeof(float)*2, &texcoords[0], GL_STATIC_DRAW);
      glEnableVertexAttribArray(texcoordloc);
      glVertexAttribPointer(texcoordloc, 2, GL_FLOAT, 0, 0, 0);
    }
    
    if (normalloc > -1)
    {
      //FIXME No Normal Support Yet
      //glBindBuffer(GL_ARRAY_BUFFER, _buffer[2]);
      //glBufferData(GL_ARRAY_BUFFER, normals.size()*sizeof(float)*2, &normals[0], GL_STATIC_DRAW);
      //glEnableVertexAttribArray(normalloc);
      //glVertexAttribPointer(normalloc, 2, GL_FLOAT, 0, 0, 0);
    }
}

void TextHandle::clear()
{   
    if(_vao[0] > -1)
	glDeleteVertexArrays(1, _vao);
  
    vertex.clear();
    texcoords.clear();
    
    _programloc  = -1;
    _vertexloc   = -1;
    _texcoordloc = -1;
    _normalloc   = -1;
    _texUnitLoc  = -1;
  
}

void TextHandle::addTriangle(uint newtexture, 
			     const glm::vec4 &vert1, const glm::vec2 &texcoord1,
			     const glm::vec4 &vert2, const glm::vec2 &texcoord2,
			     const glm::vec4 &vert3, const glm::vec2 &texcoord3)
{
  texture = newtexture;
  addVertex(vert1, texcoord1);
  addVertex(vert2, texcoord2);
  addVertex(vert3, texcoord3);
}

void TextHandle::addVertex(const glm::vec4 &newvert, const glm::vec2 &newtexcoord)
{
    vertex.push_back(newvert);
    texcoords.push_back(newtexcoord);
    
}

} //namespace ftgl
