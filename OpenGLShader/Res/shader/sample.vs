#version 460 core

in vec3 pos;
in vec2 texcoord;

uniform mat4 M;
uniform mat4 P;
uniform mat4 V;

out vec2 T_texcoord;
out vec4 T_color;

void main()
{
	T_color = vec4(vec3(0.5,0.5,0.5)+pos, 1.0); //刚好pos的值为-0.5~0.5,所有和的范围为0~1,正好是RGB值的范围
	T_texcoord = texcoord;
	gl_Position=P*V*M*vec4(pos,1.0);
}