#version 330

layout(location = 0) in vec3 vertexPosition;

out vec3 Position;

void main()
{
    Position = vertexPosition;
    gl_Position = vec4(vertexPosition, 1.0);
}
