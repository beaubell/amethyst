#version 330

uniform mat4 viewMatrix, projMatrix;

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 texcoordData; 

out vec2 Texcoord;

void main()
{
    Texcoord = texcoordData;
    gl_Position = projMatrix * viewMatrix * vec4(vertexPosition, 1.0);
}
