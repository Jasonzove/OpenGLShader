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
uniform vec3 U_EyePos;
uniform vec4 U_SpecularLight;
uniform vec4 U_SpecularMaterial;

out vec4 T_DiffuseColor;
out vec4 T_SpecularColor;

void main()
{
	//diffuse
	vec3 L = U_LightPos;
	L = normalize(L);
	vec3 N = mat3(NM)*normal;
	N = normalize(N);
	float diffuseIntensity = max(0.0, dot(L, N));
	T_DiffuseColor = U_DiffuseLight*U_DiffuseMaterial*diffuseIntensity;
	//specular
	vec3 reflectDir = reflect(-L, N);
	reflectDir = normalize(reflectDir);
	vec4 worldPos = M*vec4(pos,1.0);
	vec3 viewDir = U_EyePos - worldPos.xyz;
	viewDir = normalize(viewDir);
	float specularIntensity = pow(max(0.0, dot(reflectDir, viewDir)), 32);
	T_SpecularColor = U_SpecularLight*U_SpecularMaterial*specularIntensity;

	gl_Position = P*V*worldPos;
}