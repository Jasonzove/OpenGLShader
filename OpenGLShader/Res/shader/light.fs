#version 460 core

uniform vec4 U_AmbientLigth;
uniform vec4 U_AmbientMaterial;

uniform vec4 U_LightPos;
uniform vec4 U_DiffuseLight;
uniform vec4 U_DiffuseMaterial;

uniform vec3 U_EyePos;
uniform vec4 U_SpecularLight;
uniform vec4 U_SpecularMaterial;

//聚光灯
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
	//点光源相关
	float attenuation = 1.0; //衰减系数
	float distance = 0.0; //点到光源的位置
	float constantFactor = 0.5;
	float linearFactor = 0.3;
	float expFactor = 0.1;

	if(U_LightPos.w == 0.0) //方向光
	{
		L = U_LightPos.xyz;
	}
	else
	{
		//点光源|聚光灯(反向)
		L = U_LightPos.xyz - T_WorldPos;
	}
	L = normalize(L);
	vec3 N = normalize(T_Normal);

	float diffuseIntensity = 0.0;
	if(U_LightPos.w != 0)
	{
		if(U_CutOff > 0.0)
		{
			float radianCutOff = radians(U_CutOff);
			float cosCutoff = cos(radianCutOff);
			vec3 spotDir = normalize(U_SpotDirection.xyz);
			float cosCurrentThta = max(0.0,dot(spotDir, -L));
			if(cosCurrentThta > cosCutoff && dot(L, N) > 0.0)
			{
				diffuseIntensity = pow(cosCurrentThta, U_SpotDirection.w);
			}
		}
		else
		{
			//点光源
			distance = length(L);
			attenuation = 1.0/(constantFactor + linearFactor*distance + expFactor*distance*distance);
			diffuseIntensity = max(0.0, dot(L, N));
		}
	}

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