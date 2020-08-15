#version 460 core

uniform vec4 U_AmbientLigth;
uniform vec4 U_AmbientMaterial;

uniform vec3 U_LightPos;
uniform vec4 U_DiffuseLight;
uniform vec4 U_DiffuseMaterial;

uniform vec3 U_EyePos;
uniform vec4 U_SpecularLight;
uniform vec4 U_SpecularMaterial;

in vec3 T_Normal;
in vec3 T_WorldPos;

void main()
{
	//ambinent
	vec4 ambientColor = U_AmbientLigth*U_AmbientMaterial;

	//diffuse
	vec3 L = U_LightPos;
	L = normalize(L);
	vec3 N = normalize(T_Normal);
	float diffuseIntensity = max(0.0, dot(L, N));
	vec4 diffuseColor = U_DiffuseLight*U_DiffuseMaterial*diffuseIntensity;

	//specular
	vec3 reflectDir = reflect(-L, N);
	reflectDir = normalize(reflectDir);
	vec3 viewDir = U_EyePos - T_WorldPos;
	viewDir = normalize(viewDir);
	float specularIntensity = pow(max(0.0, dot(reflectDir, viewDir)), 32);
	vec4 specularColor = U_SpecularLight*U_SpecularMaterial*specularIntensity;

	gl_FragColor = ambientColor + diffuseColor + specularColor;
}