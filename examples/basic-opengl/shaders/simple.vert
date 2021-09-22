#version 140

in vec2 p;
void main() {
    gl_Position = vec4(p.x, p.y, 0, 1);
}
