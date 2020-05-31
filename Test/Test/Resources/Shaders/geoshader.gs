
#version 430 core 

layout (points) in; 
layout (triangle_strip, max_vertices = 9) out;

out vec3 outColor;

in VS_GS_VERTEX
{ 
	in vec4 position; 
	in vec3 color; 
	in mat4 mvp; 
}gs_in[];


void main() 
{    
	outColor = vec3(1.0f, 0.0f, 0.0f);//gs_in[0].color;

	gl_Position = gs_in[0].position + gs_in[0].mvp * vec4( 0.1,  0.1, 0.0, 0.0);   // Mid Right
    EmitVertex();
    gl_Position = gs_in[0].position + gs_in[0].mvp * vec4( 0.2, -0.2, 0.0, 0.0);  // Bot Right
    EmitVertex();
    gl_Position = gs_in[0].position + gs_in[0].mvp * vec4(-0.25,  0.1, 0.0, 0.0);   // Mid Left
    EmitVertex();
    EndPrimitive();

    gl_Position = gs_in[0].position + gs_in[0].mvp * vec4( -0.1,  0.1, 0.0, 0.0);  // Mid Left
    EmitVertex();
    gl_Position = gs_in[0].position + gs_in[0].mvp * vec4( -0.2,  -0.2, 0.0, 0.0); // Bot Left
    EmitVertex();
    gl_Position = gs_in[0].position + gs_in[0].mvp * vec4( 0.25,   0.1, 0.0, 0.0); // Mid Right
    EmitVertex();
    EndPrimitive();

    gl_Position = gs_in[0].position + gs_in[0].mvp * vec4(-0.1,  0.1, 0.0, 0.0);  // Mid Left
    EmitVertex();
    gl_Position = gs_in[0].position + gs_in[0].mvp * vec4( 0.0,  0.3, 0.0, 0.0);  // Top
    EmitVertex();
    gl_Position = gs_in[0].position + gs_in[0].mvp * vec4( 0.1,  0.1, 0.0, 0.0);  // Mid Right
	EmitVertex();
    EndPrimitive();
}