// From http://www.lighthouse3d.com/cg-topics/code-samples/opengl-3-3-glsl-1-5-sample/
#version 330

uniform mat4 viewMatrix, projMatrix;
 
layout(location = 0) in vec4 positionData;
layout(location = 1) in vec4 colorData;
 
out vec4 Color;
 
void main()
{
    Color = colorData;
    gl_Position = projMatrix * viewMatrix * positionData ;
}