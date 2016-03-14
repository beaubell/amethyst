#version 330

uniform vec4  fogColor;
uniform float fogStart;
uniform float fogEnd;

in vec3 vertcol;
in vec4 viewSpace;

layout(location = 0) out vec4 outputF;
 
void main()
{
    float dist = abs(viewSpace.z);
    float fogFactor = (fogEnd - dist)/(fogEnd - fogStart);
    fogFactor = clamp( fogFactor, 0.0, 1.0 );
 
    vec4 finalColor = mix(fogColor, vec4(vertcol, 1.0), fogFactor);
    outputF = finalColor;
}