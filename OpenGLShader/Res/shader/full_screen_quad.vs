#version 460 core

in vec3 pos;

out vec2 T_texcoord;

void main()
{
	vec4 worldPos = vec4(pos, 1.0);
	T_texcoord = pos.xy + vec2(0.5,0.5);
	worldPos.x *= 2.0;
	worldPos.y *= 2.0;
	gl_Position = worldPos;
}
