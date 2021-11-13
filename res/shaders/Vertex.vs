#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aColor;

out vec2 texCoord;
out vec3 vertCoord;
out vec3 color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    texCoord = aTexCoord;
    color = aColor;
    gl_Position = projection * view *  model * vec4(aPos, 1.0);
    vertCoord = gl_Position.xyz;
}
