#version 330 core

out vec4 FragColor;

in vec2 texCoord;
in vec3 vertCoord;
in vec3 color;

uniform sampler2D texture1;
uniform float time;

void main()
{
    //FragColor = texture(texture1, texCoord);
    FragColor = vec4(color.x, color.y, color.z, 1);
}