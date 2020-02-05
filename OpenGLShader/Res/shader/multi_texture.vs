#version 430

in vec3 pos;
in vec2 texcoord;
in vec3 normal;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

out vec2 V_Texcoord;

void main()
{	
	V_Texcoord = texcoord;
	gl_Position = P*V*M*vec4(pos, 1.0);
}