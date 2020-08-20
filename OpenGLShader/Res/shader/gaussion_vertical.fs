#version 460 core

uniform sampler2D U_MainTexture;

in vec2 T_texcoord;

void main()
{
	vec4 color = vec4(0.0);
	float texelOffset = 1/100.0f;
	float kernal[5] = {0.22,0.19,0.12,0.08,0.01};

	for(int i = 0; i < 5; ++i)
	{
		color += texture2D(U_MainTexture, vec2(T_texcoord.x, T_texcoord.y + texelOffset*i))*kernal[i];
		color += texture2D(U_MainTexture, vec2(T_texcoord.x, T_texcoord.y - texelOffset*i))*kernal[i];
	}

	gl_FragColor =  color;
}