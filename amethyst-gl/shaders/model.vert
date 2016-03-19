#version 330

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 vertexTexCoord; 

out vec3 Position;
out vec3 Normal;
out vec2 TexCoord;

uniform mat4 modelViewMatrix;
uniform mat3 normalMatrix;
uniform mat4 projMatrix;
uniform mat4 MVP;

void main()
{
    Normal = normalize(normalMatrix * vertexNormal);
    TexCoord = vertexTexCoord;
    Position = vec3( modelViewMatrix * vec4(vertexPosition, 1.0));
    gl_Position = MVP * vec4(vertexPosition, 1.0);
}
