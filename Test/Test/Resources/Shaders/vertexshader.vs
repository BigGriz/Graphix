#version 450 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;
out vec4 mWorldPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;


void main()
{
	mWorldPos = model * vec4(aPos, 1.0f);
	mWorldPos = proj * view * mWorldPos;
    //gl_Position = proj * view * mWorldPos;
	gl_Position = mWorldPos;
    TexCoord = aTexCoord;
}