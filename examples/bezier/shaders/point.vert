#version 330 core
#extension GL_ARB_explicit_uniform_location : require
#define PI 3.14

// uniform constants
layout (location = 0) uniform vec2 screen_dim;

// parameters to draw the point
layout (location = 1) in vec2 p;
layout (location = 2) in float r;
layout (location = 3) in uint n;

// parameters to pass to the fragment shader
layout (location = 4) in vec4 color;
out fragment { vec4 color; } frag;

void main()
{
    // get the appropriate triangle-fan angle for the current position
    float phi = 2.*PI*float(gl_VertexID-1)/float(n-2u);

    // the first position is drawn at the center (radius 0)
    // so r should be 0 if gl_Position = 0, and r otherwise
    vec2 q = p + r*float(gl_VertexID > 0)*vec2(cos(phi), sin(phi));

    gl_Position = vec4(q.xy/screen_dim, 0., 1.) - vec4(.5, .5, 0., 0.);

    // make sure to pass the color along
    frag.color = color;
}
