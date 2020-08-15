#version 460 core
//Öð¶¥µãäÖÈ¾

in vec3 pos;
in vec2 texcoord;
in vec3 normal;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform mat4 NM;

uniform vec3 U_LightPos;

out vec4 T_CartoonColor;

void main()
{
	//diffuse
	vec3 L = U_LightPos;
	L = normalize(L);
	vec3 N = mat3(NM)*normal;
	N = normalize(N);
	float intensity = max(0.0, dot(L, N));
	if(intensity > 0.5)
	{
		T_CartoonColor = vec4(0.4,0.4,0.4,1.0);
	}
	else
	{
		T_CartoonColor = vec4(0.1,0.1,0.1,1.0);
	}

	gl_Position = P*V*M*vec4(pos, 1.0);
}