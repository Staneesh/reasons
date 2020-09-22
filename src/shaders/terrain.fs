#version 330 core
out vec4 FragColor;

in vec4 pos;

void main()
{
    FragColor = vec4(0.5f, pos.z - 0.3f, 0.0f, 1.0f);
};