#version 450 core

layout (points) in;
layout (points, max_vertices = 1) out;

in VS_GS_VERTEX{ 
	in vec4 mWorldPos; 
	in vec2 TexCoord;
} gs_in[];

	out vec2 TexCoord;
	out vec4 mWorldPos;

void main() 
{    
	TexCoord = gs_in[0].TexCoord;
	mWorldPos = gs_in[0].mWorldPos;

    EmitVertex();
    EndPrimitive();
}  