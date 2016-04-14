#version 330

layout(location = 0) in vec3 vertexPosition;

uniform float eye;

out vec2 Texcoord;

void main()
{
    float eyediv = 1.0 - 0.5*abs(eye);

    Texcoord = (vertexPosition.xy + 1.0 )*0.5;
    vec3 Position = vec3((vertexPosition.x + eye)*eyediv, vertexPosition.yz);
    gl_Position = vec4(Position, 1.0);
}
