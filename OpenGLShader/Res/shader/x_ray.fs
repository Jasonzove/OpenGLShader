#version 460 core

uniform vec3 U_EyePos;

in vec3 T_Normal;
in vec3 T_WorldPos;

void main()
{
	vec3 L = U_EyePos-T_WorldPos;
	L = normalize(L);
	vec3 N = normalize(T_Normal);
	float cosThta = dot(L,N);
	float alpha = 0.0;
	if(cosThta > 0.0)
	{
		alpha = pow(1-cosThta, 3);
	}

	gl_FragColor = vec4(1.0,1.0,1.0,alpha);
}