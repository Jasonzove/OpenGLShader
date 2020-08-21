#version 460 core

uniform sampler2D U_MainTexture;

in vec2 T_texcoord;

void main()
{
    vec4 color = texture2D(U_MainTexture, T_texcoord);
    if(color.a > 1.0)
    {
        discard;
	}

    gl_FragColor = color;
}