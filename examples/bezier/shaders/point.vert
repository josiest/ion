#version 330 core
#define PI 3.14

// parameters to draw the point
layout (location = 0) in vec2 p;
layout (location = 1) in float r;
layout (location = 2) in uint n;

// parameters to pass to the fragment shader
layout (location = 3) in vec4 color;
out fragment { vec4 color; } frag;

void main()
{
    // the first position is drawn at the center (radius 0)
    // so r should be 0 if gl_Position = 0, and r otherwise
    float R = r*float(gl_VertexID > 0);

    // get the appropriate triangle-fan angle for the current position
    float phi = 2.*PI*float(gl_VertexID-1)/float(n-2u);

    // the point should be drawn at the input point
    // and offset by the triangle-fan vertex
    gl_Position = vec4(p.xy, 0., 1.) + vec4(R*cos(phi), R*sin(phi), 0., 0.);

    // make sure to pass the color along
    frag.color = color;
}
