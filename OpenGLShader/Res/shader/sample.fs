#version 460

in vec4 T_color;
in vec4 T_texcoord;

uniform sampler2D U_MainTexture;

void main()
{
	gl_FragColor=T_color*texture2D(U_MainTexture,T_texcoord.xy);
}