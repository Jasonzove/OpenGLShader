#version 460 core

uniform vec4 U_AmbientLigth;
uniform vec4 U_AmbientMaterial;

in vec4 T_CartoonColor;

void main()
{
	gl_FragColor = U_AmbientLigth*U_AmbientMaterial + T_CartoonColor;
}