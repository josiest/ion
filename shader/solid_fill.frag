#version 330 core
out vec4 frag_color;
uniform vec4 fill_color;

void main() {
    frag_color = fill_color;
}