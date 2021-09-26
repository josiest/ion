#version 330 core

in fragment { vec4 color; } frag;
out vec4 color;

void main()
{
    color = frag.color;
}
