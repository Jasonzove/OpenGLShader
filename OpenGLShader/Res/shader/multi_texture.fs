#version 430

in vec2 V_Texcoord;

uniform sampler2D U_MainTexture;
uniform sampler2D U_SecondTexture;

void main()
{
    gl_FragColor = texture2D(U_MainTexture, V_Texcoord) * texture2D(U_SecondTexture, V_Texcoord);
}