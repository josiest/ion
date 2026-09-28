#version 330 core
layout (location = 0) in vec2 position;
uniform mat4x4 transform;

void main()
{
    gl_Position = transform * vec4(position, 0., 1.);
}