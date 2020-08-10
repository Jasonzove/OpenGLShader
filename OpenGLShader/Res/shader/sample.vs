#version 460 core

in vec3 pos;
in vec2 texcoord;

uniform mat4 M;
uniform mat4 P;
uniform mat4 V;

out vec2 T_texcoord;

void main()
{
	T_texcoord = texcoord;
	gl_Position=P*V*M*vec4(pos,1.0);
}