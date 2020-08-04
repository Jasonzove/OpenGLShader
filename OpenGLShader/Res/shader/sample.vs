#version 460

in vec3 pos;
in vec4 color;
in vec4 texcoord;

uniform mat4 M;
uniform mat4 P;
uniform mat4 V;

out vec4 T_color;
out vec4 T_texcoord;

void main()
{
	T_color=color;
	T_texcoord=texcoord;
	gl_Position=P*V*M*vec4(pos,1.0);
}