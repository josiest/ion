#version 330 core
in vec2 uv;
out vec4 frag_color;

uniform vec4 fill_color;
uniform vec2 bounds;
uniform float rounding;

void main()
{
    frag_color = vec4(vec3(uv.x), 1.f);
    // vec2 signed_uv = 2.f*uv -1.f;
    // vec2 normal_bounds = bounds/max(bounds.x, bounds.y);

    // float rounding_factor = clamp(rounding, 0.f, 1.f) * min(normal_bounds.x, normal_bounds.y);
    // vec2 rect_sdf_bounds = normal_bounds * (abs(signed_uv) - 1.f) + rounding_factor;

    // float rect_sdf = length(max(rect_sdf_bounds, 0.f));
    // rect_sdf = step(rect_sdf, rounding_factor);
    // frag_color = fill_color * rect_sdf;
}