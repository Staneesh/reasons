#version 330 core
out vec4 FragColor;

in vec4 pos;
in vec3 world_pos;

void main()
{
    float r = clamp(0.4f - world_pos.y, 0.0f, 1.0f);
    float g = 0.0f;
    float b = clamp(world_pos.y + 0.4f, 0.0f, 1.0f);
    
    FragColor = vec4(r, g, b, 1.0f);
};