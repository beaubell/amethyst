// From http://www.lighthouse3d.com/cg-topics/code-samples/opengl-3-3-glsl-1-5-sample/
#version 330

uniform mat4 viewMatrix, projMatrix;

layout(location = 0) in vec4 vertexData;
layout(location = 1) in vec2 texcoordData; 

out vec2 Texcoord;

void main()
{
    Texcoord = texcoordData;
    gl_Position = projMatrix * viewMatrix * vertexData ;
}
