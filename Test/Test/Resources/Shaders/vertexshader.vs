#version 450 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;
out vec4 mWorldPos;

/*out VS_GS_VERTEX{ 
	out vec4 mWorldPos; 
	out vec2 TexCoord;
} vs_out;*/

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
	vec4 mWorldPos = model * vec4(aPos, 1.0f);
	mWorldPos = proj * view * mWorldPos;
    //gl_Position = proj * view * mWorldPos;
	gl_Position = mWorldPos;
    TexCoord = aTexCoord;

	//vs_out.TexCoord = aTexCoord;
	//vs_out.mWorldPos = mWorldPos;
}