// From http://www.lighthouse3d.com/cg-topics/code-samples/opengl-3-3-glsl-1-5-sample/
#version 330

uniform sampler2D baseTex;
uniform vec4 color;

in vec2 Texcoord;

layout(location = 0) out vec4 outputF;
 
void main()
{
    vec4 texel;
    texel.a = texture2D(baseTex, Texcoord.st).r * color.a;
    texel.rgb = color.rgb;
    outputF = texel;
}