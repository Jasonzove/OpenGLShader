#version 460 core

in vec3 pos;
in vec2 texcoord;

out vec2 T_texcoord;

void main()
{
	vec4 worldPos = vec4(pos, 1.0);
	T_texcoord = texcoord;
	worldPos.x *= 2.0;
	worldPos.y *= 2.0;
	gl_Position = worldPos;
}
