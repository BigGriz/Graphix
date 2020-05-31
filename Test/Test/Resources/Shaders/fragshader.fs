#version 450 core

out vec4 FragColor;
  
in vec4 mWorldPos; 
in vec2 TexCoord;

uniform sampler2D texture1;
//uniform sampler2D texture2;

uniform vec3 cameraPos;
uniform bool fog;

void main()
{
    //FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord));

	if (fog)
	{
		float d = distance(mWorldPos.xyz, cameraPos);
		float lerp = (d - 5.0f) / 10.0f;
		lerp = clamp(lerp, 0.0f, 1.0f);

		vec4 vFogColor = vec4(0.1f, 0.1f, 0.1f, 1.0f);

		FragColor = mix(texture(texture1, TexCoord), vFogColor, lerp);
	}
	else
	{
		FragColor = texture(texture1, TexCoord);
	}
}