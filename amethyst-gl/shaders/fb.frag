#version 330

in vec2 Texcoord;

uniform sampler2D baseTex;

layout(location = 0) out vec4 FragColor;

void main()
{
    FragColor = texture(baseTex, Texcoord.st);
}