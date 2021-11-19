//#version 330 core
//layout (points) in;
//layout (points, max_vertices = 1) out;
//
//void main()
//{
//    gl_Position = gl_in[0].gl_Position;
//    EmitVertex();
//    EndPrimitive();
//}
//
#version 330 core
layout (points) in;
layout (triangle_strip) out;
layout (max_vertices = 4) out;

uniform float radius;
uniform ivec2 vp_size;

out vec3 center;
out vec3 coord;
out vec2 n_radius;

void main()
{
    n_radius = radius / vp_size;
    vec4 pos = gl_in[0].gl_Position;
    center = pos.xyz;
    gl_Position = vec4(pos.x - n_radius.x, pos.y + n_radius.y, pos.z, 1.0);
    coord = gl_Position.xyz;
    EmitVertex();

    gl_Position = vec4(pos.x + n_radius.x, pos.y + n_radius.y, pos.z, 1.0);
    center = pos.xyz;
    coord = gl_Position.xyz;
    EmitVertex();

    gl_Position = vec4(pos.x - n_radius.x, pos.y - n_radius.y, pos.z, 1.0);
    center = pos.xyz;
    coord = gl_Position.xyz;
    EmitVertex();

    gl_Position = vec4(pos.x + n_radius.x, pos.y - n_radius.y, pos.z, 1.0);
    center = pos.xyz;
    coord = gl_Position.xyz;
    EmitVertex();

    EndPrimitive();
}