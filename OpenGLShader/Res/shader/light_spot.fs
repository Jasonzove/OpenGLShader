#version 460 core

uniform vec4 U_AmbientLigth;
uniform vec4 U_AmbientMaterial;

uniform vec4 U_LightPos;
uniform vec4 U_DiffuseLight;
uniform vec4 U_DiffuseMaterial;

uniform vec3 U_EyePos;
uniform vec4 U_SpecularLight;
uniform vec4 U_SpecularMaterial;

//�۹��
uniform vec4 U_SpotDirection;
uniform float U_CutOff;

in vec3 T_Normal;
in vec3 T_WorldPos;

void main()
{
	//ambinent
	vec4 ambientColor = U_AmbientLigth*U_AmbientMaterial;

	vec3 L = vec3(0.0);
	//diffuse
	//�۹��
	float radianCutOff = radians(U_CutOff);
	float cosCutoff = cos(radianCutOff);

	if(U_LightPos.w == 0.0) //�����
	{
		L = U_LightPos.xyz;
	}
	else
	{
		//�۹��
		L = T_WorldPos - U_LightPos.xyz;
	}

	L = normalize(L);
	vec3 spotDir = normalize(U_SpotDirection.xyz);
	float cosCurrentThta = max(0.0,dot(spotDir, L));
	float spotFactor = 0.0;
	if(cosCurrentThta > cosCutoff)
	{
		spotFactor = 0.8;
	}

	vec4 diffuseColor = U_DiffuseLight*U_DiffuseMaterial*spotFactor;

	gl_FragColor = ambientColor + diffuseColor;
}