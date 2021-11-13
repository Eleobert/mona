#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 inormal;

uniform mat4 mvp;

flat out vec3 flat_normal;
out vec3 normal;

out vec3 coord;

void main()
{
    gl_Position = mvp * vec4(pos, 1.0);
    normal = inormal;
    flat_normal = inormal;
    coord = pos;
}
