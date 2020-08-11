#version 460 core

uniform sampler2D U_MainTexture;

in vec2 T_texcoord;

void main()
{
    gl_FragColor = texture2D(U_MainTexture, T_texcoord);
}