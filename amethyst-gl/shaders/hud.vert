#version 330

uniform mat4 viewMatrix, projMatrix;

layout(location = 0) in vec3 vertexLocation;
layout(location = 1) in vec3 vertexColor;

out vec3 vertcol;
out vec4 viewSpace;

void main()
{
    viewSpace = viewMatrix * vec4(vertexLocation, 1.0);
    gl_Position = projMatrix * viewSpace;
    vertcol = vertexColor;
}
