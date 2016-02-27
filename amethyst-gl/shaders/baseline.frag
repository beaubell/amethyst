// From http://www.lighthouse3d.com/cg-topics/code-samples/opengl-3-3-glsl-1-5-sample/
#version 330

in vec4 Color;
layout(location = 0) out vec4 outputF;
 
void main()
{
    outputF = Color;
}