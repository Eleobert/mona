#version 330 core


/**
 * This could simpler if we wouldn't want to
 * keep the dots size when the screen is resized.
 */

out vec4 FragColor;
in vec3 coord;
in vec3 center;
in vec2 n_radius;

uniform float radius;
uniform vec4 color;

void main()
{
    float inner_radius = length(n_radius) * 0.02;
    vec2 dist = center.xy - coord.xy;
    dist /= n_radius;

    if(length(dist) < 1)
    {
        float alpha = smoothstep(1,  1 - 2/radius, length(dist));
        FragColor = vec4(color.rgb, color.a * alpha);
    }
    else discard;
}