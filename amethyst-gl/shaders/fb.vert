#version 330

layout(location = 0) in vec3 vertexPosition;

uniform float eye;

out vec2 Texcoord;

void main()
{
    Texcoord = (vertexPosition.xy + 1.0 )/2.0;
    vec3 Position = vec3((vertexPosition.x + eye)/2.0, vertexPosition.yz);
    gl_Position = vec4(Position, 1.0);
}
