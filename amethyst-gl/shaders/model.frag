#version 330

uniform sampler2D baseTex;

in vec2 Texcoord;

layout(location = 0) out vec4 outputF;
 
void main()
{
    outputF = texture2D(baseTex, Texcoord.st);
}