#version 460 core

uniform vec4 U_AmbientLigth;
uniform vec4 U_AmbientMaterial;

uniform vec4 U_LightPos;
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

	vec3 L = vec3(0.0);
	//diffuse
	//���Դ���
	float attenuation = 1.0; //˥��ϵ��
	float distance = 0.0; //�㵽��Դ��λ��
	float constantFactor = 0.5;
	float linearFactor = 0.3;
	float expFactor = 0.1;

	if(U_LightPos.w == 0.0) //�����
	{
		L = U_LightPos.xyz;
	}
	else
	{
		//���Դ
		L = U_LightPos.xyz - T_WorldPos;
		distance = length(L);
		attenuation = 1.0/(constantFactor + linearFactor*distance + expFactor*distance*distance);
	}

	L = normalize(L);
	vec3 N = normalize(T_Normal);
	float diffuseIntensity = max(0.0, dot(L, N));
	vec4 diffuseColor = U_DiffuseLight*U_DiffuseMaterial*(diffuseIntensity*attenuation);

	//specular
	vec3 reflectDir = reflect(-L, N);
	reflectDir = normalize(reflectDir);
	vec3 viewDir = U_EyePos - T_WorldPos;
	viewDir = normalize(viewDir);
	float specularIntensity = pow(max(0.0, dot(reflectDir, viewDir)), 32);
	vec4 specularColor = U_SpecularLight*U_SpecularMaterial*(specularIntensity*attenuation);

	gl_FragColor = ambientColor + diffuseColor + specularColor;
}