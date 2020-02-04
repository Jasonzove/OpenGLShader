#version 430

in vec3 pos;
in vec2 texcoord;
in vec3 normal;

uniform mat4 M;
uniform mat4 P;
uniform mat4 V;

out vec2 V_Texcoord;
out vec4 V_Color;

void main()
{
	V_Texcoord=texcoord;
	V_Color = vec4(vec3(0.5, 0.5, 0.5)+pos, 1.0);
	gl_Position=P*V*M*vec4(pos,1.0);
}