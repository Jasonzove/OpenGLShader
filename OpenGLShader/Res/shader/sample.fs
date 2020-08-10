#version 460 core

in vec2 T_texcoord;
in vec4 T_color;

uniform sampler2D U_MainTexture;

void main()
{
	gl_FragColor = T_color*texture2D(U_MainTexture,T_texcoord);
}