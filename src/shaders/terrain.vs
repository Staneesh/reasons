#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 normal_in;

out vec3 world_pos;
out vec3 normal_pass;
out float visibility;

uniform mat4 projection_view;

const float density = 0.5;
const float gradient = 1.5;

void main()
{
	vec4 position_relative_to_camera = projection_view * vec4(aPos, 1.0f);
	gl_Position = position_relative_to_camera;
	world_pos = aPos;
	normal_pass = normal_in;

	float fragment_camera_dist = length(position_relative_to_camera.xyz);
	visibility = exp(-pow(fragment_camera_dist*density, gradient));
	visibility = clamp(visibility, 0.0f, 1.0f);


}