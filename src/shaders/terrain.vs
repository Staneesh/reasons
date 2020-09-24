#version 330 core
layout (location = 0) in vec3 aPos;

out vec4 pos;
out vec3 world_pos;

uniform mat4 projection_view;

void main()
{
	pos = projection_view * vec4(aPos, 1.0f);
	gl_Position = pos;//vec4(aPos, 1.0f);
	world_pos = aPos;
}