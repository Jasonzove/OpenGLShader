#version 460 core
//ÖğÏñËØäÖÈ¾

in vec3 pos;
in vec2 texcoord;
in vec3 normal;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform mat4 NM;

out vec3 T_Normal;

void main()
{
	T_Normal = mat3(NM)*normal;
	gl_Position = P*V*M*vec4(pos, 1.0);
}