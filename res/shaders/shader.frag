#version 330 core

out vec4 FragColor;
flat in vec3 flat_normal;
in vec3 normal;
in vec3 coord;
uniform int render_mode;



uniform sampler2D texture1;

void main()
{
    if(abs(coord.y) > 1) 
        discard;
    
    vec3 color;
    
    if(render_mode == 0)
    {
        color = abs(vec3(normal.rbg));
    }
    else if(render_mode == 1)
    {
        color = abs(vec3(flat_normal.rbg));
    }
    else color = vec3(0.5f);
    
    FragColor = vec4(color, 1);
}