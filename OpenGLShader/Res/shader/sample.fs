#version 460 core

in vec2 T_texcoord;

uniform sampler2D U_MainTexture;
uniform sampler2D U_SecondaryTexture;

void main()
{
	gl_FragColor = vec4(vec3(texture2D(U_MainTexture,T_texcoord).rgb)*vec3(texture2D(U_SecondaryTexture, T_texcoord).rgb),1.0);
}