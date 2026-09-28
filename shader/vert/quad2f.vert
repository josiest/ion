#version 330 core
layout (location = 0) in vec2 position;
layout (location = 1) in vec2 vert_uv;

out vec2 frag_uv;
uniform mat4 transform;

void main()
{
    gl_Position = transform * vec4(position, 0.f, 1.f);
    frag_uv = vert_uv;
}