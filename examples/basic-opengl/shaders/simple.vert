#version 140

in vec2 p;
int main() {
    gl_Position = vec4(p.x, p.y, 0, 1);
}
