#version 460 core

in vec2 T_texcoord;

uniform sampler2D U_MainTexture;

void main()
{
	gl_FragColor = texture2D(U_MainTexture,T_texcoord);
}