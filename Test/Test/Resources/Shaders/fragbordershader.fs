#version 450 core

out vec4 FragColor;

in vec2 TexCoord;
in vec4 mWorldPos;

uniform sampler2D texture1;
uniform sampler2D texture2;

uniform vec3 cameraPos;
uniform bool fog;

void main()
{
   // FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2) * vec4(0.0f, 0.0f, 0.0f, 1.0f);
	//FragColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	if (fog)
	{
		float d = distance(mWorldPos.xyz, cameraPos);
		float lerp = (d - 5.0f) / 10.0f;
		lerp = clamp(lerp, 0.0f, 1.0f);

		vec4 vFogColor = vec4(0.1f, 0.1f, 0.1f, 1.0f);

		FragColor = mix((texture(texture1, TexCoord)  * vec4(0.0f, 0.0f, 0.0f, 1.0f)), vFogColor, lerp);
	}
	else
	{
		FragColor = texture(texture1, TexCoord);
	}
}