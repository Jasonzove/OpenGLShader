#version 430

in vec2 V_Texcoord;

uniform sampler2D U_MainTexture;

void main()
{
	gl_FragColor=texture2D(U_MainTexture,V_Texcoord);
}