#version 330
layout (location = 0) in vec2 local_vertex;
uniform mat4x4 transform;

void main()
{
    gl_Position = transform * vec4(local_vertex, 0., 1.);
}