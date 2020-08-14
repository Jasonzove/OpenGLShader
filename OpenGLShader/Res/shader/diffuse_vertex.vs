#version 460 core
//÷∂•µ„‰÷»æ

in vec3 pos;
in vec2 texcoord;
in vec3 normal;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform mat4 NM;

uniform vec3 U_LightPos;
uniform vec4 U_DiffuseLight;
uniform vec4 U_DiffuseMaterial;

out vec4 T_DiffuseColor;

void main()
{
	vec3 L = U_LightPos;
	L = normalize(L);
	vec3 N = mat3(NM)*normal;
	N = normalize(N);
	float diffuseIntensity = max(0.0, dot(L, N));
	T_DiffuseColor = U_DiffuseLight*U_DiffuseMaterial*diffuseIntensity;

	gl_Position = P*V*M*vec4(pos, 1.0);
}