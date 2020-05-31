#version 450 core

layout (location = 0) in vec3 position;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

out vec3 v_Color;

void main()
{
	float height = position.y;
	v_Color = vec3(height + 0.25f, 0.75f - height, height);
	gl_Position = projection * view * vec4(position, 1.0f);
}