#version 330 core

out vec4 FragColor;
in vec3 coord;


uniform sampler2D texture1;

void main()
{
    FragColor = vec4(coord, 1);
}