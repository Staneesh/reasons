#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 pos;

void main()
{
	pos = projection * view * model * vec4(aPos, 1.0f);
	gl_Position = pos;//vec4(aPos, 1.0f);
}