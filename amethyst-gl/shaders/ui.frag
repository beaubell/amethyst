#version 330

uniform vec4 color;

layout(location = 0) out vec4 outputF;
 
void main()
{
    outputF = color;
    //outputF = vec4(1.0,1.0,1.0,1.0) + color;
}