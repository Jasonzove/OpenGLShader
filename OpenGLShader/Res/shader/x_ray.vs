#version 460 core
//÷œÒÀÿ‰÷»æ

in vec3 pos;
in vec2 texcoord;
in vec3 normal;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform mat4 NM;

out vec3 T_Normal;
out vec3 T_WorldPos;

void main()
{
	T_Normal = mat3(NM)*normal;
	T_WorldPos = (M*vec4(pos, 1.0)).xyz;
	gl_Position = P*V*vec4(T_WorldPos,1.0);
}