// From http://www.lighthouse3d.com/cg-topics/code-samples/opengl-3-3-glsl-1-5-sample/
#version 330

uniform mat4 viewMatrix, projMatrix;

layout(location = 0) in vec4 vertexData;

void main()
{
    gl_Position = projMatrix * viewMatrix * vertexData ;
}
