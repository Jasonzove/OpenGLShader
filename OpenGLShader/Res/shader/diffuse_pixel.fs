#version 460 core

uniform vec4 U_AmbientLigth;
uniform vec4 U_AmbientMaterial;

uniform vec3 U_LightPos;
uniform vec4 U_DiffuseLight;
uniform vec4 U_DiffuseMaterial;

in vec3 T_Normal;

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

	gl_FragColor = ambientColor + diffuseColor;
}