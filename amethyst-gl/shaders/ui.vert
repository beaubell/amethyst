#version 330

uniform mat4 viewMatrix, projMatrix;

layout(location = 0) in vec4 vertexData;

void main()
{
    gl_Position = projMatrix * viewMatrix * vertexData ;
}
