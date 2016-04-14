#version 330

in vec3 Position;

uniform sampler2D baseTex;

layout(location = 0) out vec4 FragColor;

void main()
{
    vec2 TexCoord = (Position.xy + 1.0 )/2.0;
    FragColor = texture(baseTex, TexCoord.st);
}