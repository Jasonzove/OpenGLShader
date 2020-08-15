#version 460 core

uniform vec4 U_AmbientLigth;
uniform vec4 U_AmbientMaterial;

uniform vec3 U_LightPos;

in vec3 T_Normal;

void main()
{
	//ambinent
	vec4 ambientColor = U_AmbientLigth*U_AmbientMaterial;

	//diffuse
	vec3 L = U_LightPos;
	L = normalize(L);
	vec3 N = normalize(T_Normal);
	float intensity = max(0.0, dot(L, N));
	vec4 cartoonColor;
	if(intensity > 0.5)
	{
		cartoonColor = vec4(0.4,0.4,0.4,1.0);
	}
	else
	{
		cartoonColor = vec4(0.1,0.1,0.1,1.0);
	}

	gl_FragColor = ambientColor + cartoonColor;
}