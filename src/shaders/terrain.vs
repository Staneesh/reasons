#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 normal_pass;

out vec3 world_pos;
out vec3 normal;

uniform mat4 projection_view;


void main()
{
	gl_Position = projection_view * vec4(aPos, 1.0f);
	world_pos = aPos;
	normal = normal_pass;
}